// Assault Cube Trainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Bypass.h"
#include <vector>
#include "playerEnt.h"

DWORD viewMatrix = 0x501AE8;
DWORD numOfPlayersAddress = 0x400000 + 0x10F500;
DWORD basePlayerAddressPointer = 0x400000 + 0x109B74;
DWORD botArrayPointer = 0x50F4F8;
HWND hwndAC_Client;
HBRUSH Brush;
HDC hdcAC_client;
HFONT Font;
float Matrix[16];
COLORREF TextCOLOR;
COLORREF TextCOLORRED;
//These two function are from a old fleep post, i was able to use them to create the box style however this was just to save time :) Thx Fleep
void DrawFilledRect(int x, int y, int w, int h)
{
	RECT rect = { x, y, x + w, y + h };
	FillRect(hdcAC_client, &rect, Brush);
}


void DrawBorderBox(int x, int y, int w, int h, int thickness)
{

	DrawFilledRect(x, y, w, thickness);

	DrawFilledRect(x, y, thickness, h);

	DrawFilledRect((x + w), y, thickness, h);

	DrawFilledRect(x, y + h, w + thickness, thickness);
}


void DrawLine(int targetX, int targetY)
{
	MoveToEx(hdcAC_client, 960, 1080, NULL);
	LineTo(hdcAC_client, targetX, targetY);

}


void DrawString(int x, int y, COLORREF color, const char* text)
{
	SetTextAlign(hdcAC_client, TA_CENTER | TA_NOUPDATECP);

	SetBkColor(hdcAC_client, RGB(0, 0, 0));
	SetBkMode(hdcAC_client, TRANSPARENT);

	SetTextColor(hdcAC_client, color);

	SelectObject(hdcAC_client, Font);

	TextOutA(hdcAC_client, x, y, text, strlen(text));

	DeleteObject(Font);
}
//this is the world to screen function for openGL
bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight) // 3D to 2D
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	Vec4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	//Transform to window coordinates
	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

using namespace std;

int main()
{

	TextCOLOR = RGB(255, 255, 255);

	Bypass bypass;
	DWORD pid;
	DWORD ammoAddress;
	
	DWORD basePlayerAddress;
	DWORD bot_Array_Address;
	DWORD currBotAddress;
	DWORD currAddress;
	
	//DWORD Health = 0xf8;
	//DWORD Armor = 0xfc;
	//DWORD rifle_ammo = 0x150;
	//DWORD pistol_ammo = 0x13c;
	//DWORD akimbo_ammo = 0x15c;
	//DWORD grenade_ammo = 0x158;
	//DWORD sniper_ammo = 0x14c;

	playerEnt human_Player;
	playerEnt bots[31];

	/*vector<DWORD> addressVector;
	addressVector.push_back(Health);
	addressVector.push_back(Armor);
	addressVector.push_back(rifle_ammo);
	addressVector.push_back(pistol_ammo);
	addressVector.push_back(akimbo_ammo);
	addressVector.push_back(grenade_ammo);
	addressVector.push_back(sniper_ammo);*/


	int ammoCount;
	int choice;
	int buffer;
	int newAmount = 1337; 
	int numOfPlayers;

	LPCSTR str = "AssaultCube";
	HWND hWnd = FindWindowA(0, str);
	if (hWnd == NULL) {
		cout << "Window not found" << endl;
		Sleep(3000);
		exit(-1);
	}
	GetWindowThreadProcessId(hWnd, &pid);
	if (pid == NULL)
	{
		cout << "process id not found" << endl;
		Sleep(3000);
		exit(-1);
	}
	if (bypass.Attach(pid))
		cout << "Attached successfully!" << endl;
	else {
		cout << "Failed to attach." << endl;
		system("pause");
		return EXIT_FAILURE;
	}


	//Sets the amount of Players
	if (!bypass.Read(numOfPlayersAddress, &numOfPlayers, sizeof(numOfPlayers)))
	{
		cout << "Failed to read number of players!";
		system("pause");
		return EXIT_FAILURE;
	}

	if (bypass.Read(botArrayPointer, &bot_Array_Address, sizeof(bot_Array_Address)))
	{
		cout << "bot array address: " << hex << bot_Array_Address << endl;
	}
	else
	{
		cout << "Failed to read bot array address!" << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	if (bypass.Read(basePlayerAddressPointer, &basePlayerAddress, sizeof(basePlayerAddress)))
	{
		cout << "Base player address: " << hex << basePlayerAddress << endl;
	}
	else
	{
		cout << "Failed to read!" << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	while (true) 
	{
		bypass.Read(viewMatrix, &Matrix, sizeof(Matrix));
		//memcpy(&Matrix, (PBYTE*)(viewMatrix), sizeof(Matrix));
		hdcAC_client = GetDC(hWnd);
		// Base of Player
		Vec2 vScreen;
		// Head of player
		Vec2 vHead;

		human_Player.loadPlayerEnt(bypass, basePlayerAddress);
		human_Player.printToConsole();
		cout << endl;

		for (int i = 0; i < numOfPlayers-1; ++i)
		{
			if (bypass.Read((bot_Array_Address + (0x4 * (i+1))), &currBotAddress, sizeof(currBotAddress)))
			{
				if (bots[i].loadPlayerEnt(bypass, currBotAddress))
				{
				/*bots[i].printToConsole();
				cout << "distance to PC: " << dec << bots[i].distanceToPlayer(human_Player.pos) << endl << endl;*/
				if (WorldToScreen(bots[i].m_FootPos, vScreen, Matrix, 1024, 768))
				{
					if (WorldToScreen(bots[i].m_HeadPos, vHead, Matrix, 1024, 768))
					{
						//Creates the head height
						float head = vHead.y - vScreen.y;
						//Creates Width
						float width = head / 2;
						//Creates Center
						float center = width / -2;
						//Creates Extra area above head
						float extra = head / -6;

						//Sets the Brush Color
						Brush = CreateSolidBrush(RGB(158, 66, 244));
						//Draws the box
						DrawBorderBox(vScreen.x + center, vScreen.y, width, head - extra, 1);
						DeleteObject(Brush);

						//Draw our health by converting a int to a char
						char healthChar[255];
						sprintf_s(healthChar, sizeof(healthChar), " %i", (int)(bots[i].m_Health));
						DrawString(vScreen.x, vScreen.y, TextCOLOR, healthChar);
						DrawString(vScreen.x, vScreen.y - 20, TextCOLOR, bots[i].m_Name);

						//Turns on snaplines
						if (GetKeyState(VK_F2) & 1)
						{
							DrawLine(vScreen.x, vScreen.y);
						}
					}
				}
				}
				else
				{
					cout << "Failed to read status!";
					system("pause");
					return EXIT_FAILURE;
				}
			}
			else
			{
				cout << "Failed to read bot array address!" << endl;
				system("pause");
				return EXIT_FAILURE;
			}
		}
		Sleep(1);
		DeleteObject(hdcAC_client);
		system("CLS");
	}

	

	//while (true) 
	//{
	//	for (DWORD d : addressVector)
	//	{
	//		currAddress = basePlayerAddress + d;
	//		if (bypass.Write(currAddress, &newAmount, sizeof(newAmount)));
	//			
	//		else
	//		{
	//			cout << "Failed to overwrite!" << endl;
	//			system("pause");
	//			return EXIT_FAILURE;
	//		}
	//	}
	//	Sleep(150);
	//}


	
		/*cout << "Enter address: ";
		cin >> hex >> address;
		cout << endl;

	
		cout << "Enter 1 for read 2 for write: ";
		cin >> choice;



		switch (choice)
		{
			case 1 : 
				if (bypass.Read(address, &buffer, sizeof(buffer)))
					cout << "Read value: " << buffer << endl;
				else
					cout << "Failed to read." << endl;
				break;

			case 2 :
				cout << "Enter value to write: ";
				cin >> dec >> buffer;
				if (bypass.Write(address, &buffer, sizeof(buffer)))
					cout << "\nValue overwritten successfully!" << endl;
				else
					cout << "\nFailed to overwrite value." << endl;
				break;
		
			default :
				cout << "\nCouldn't decide read / write";
		}

		cout << "\nTo restart enter 1, to stop enter 0: ";
		cin >> choice;
		if (choice == 0)
			flag = FALSE;
	}*/
}
