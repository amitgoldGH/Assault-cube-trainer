#include "playerEnt.h"
/*
BOOL playerEnt::loadPlayerEnt(Bypass& bypass, uintptr_t playerAddress)
{
	if (!bypass.Read(playerAddress, &vTable, sizeof(vTable)))
		return FALSE;
	if (!bypass.Read(playerAddress + 0x4, &head, sizeof(head)))
		return FALSE;
	if (!bypass.Read(playerAddress + 0x34, &pos, sizeof(pos)))
		return FALSE;
	if (!bypass.Read(playerAddress + 0x40, &angle, sizeof(angle)))
		return FALSE;
	if (!bypass.Read(playerAddress + 0x71, &bScoping, sizeof(bScoping)))
		return FALSE;
	if (!bypass.Read(playerAddress + 0xF8, &health, sizeof(health)))
		return FALSE;
	if (!bypass.Read(playerAddress + 0xFC, &armor, sizeof(armor)))
		return FALSE;
	if (!bypass.Read(playerAddress + 0x224, &bAttacking, sizeof(bAttacking)))
		return FALSE;
	if (!bypass.Read(playerAddress + 0x225, &name, sizeof(name)))
		return FALSE;
	if (!bypass.Read(playerAddress + 0x32C, &team, sizeof(team)))
		return FALSE;
	if (!bypass.Read(playerAddress + 0x338, &state, sizeof(state)))
		return FALSE;
	return TRUE;
}*/

BOOL playerEnt::loadPlayerEnt(Bypass& bypass, uintptr_t playerAddress)
{
	if (!bypass.Read(playerAddress, this, sizeof(playerEnt)))
		return FALSE;
}

void playerEnt::printToConsole()
{
	cout << "Player name: " << m_Name << endl;
	cout << "Position: " << m_HeadPos.x << "," << m_HeadPos.y << "," << m_HeadPos.z << endl;
	cout << "Health: " << dec << m_Health << endl;
	cout << "Armor : " << dec << m_Armor << endl;
}

float playerEnt::distanceToPlayer(Vec3 other_Position)
{
	float distance = sqrtf(
	((other_Position.x - this->m_FootPos.x) * (other_Position.x - this->m_FootPos.x))
		+ ((other_Position.y - this->m_FootPos.y) * (other_Position.y - this->m_FootPos.y))
		+ ((other_Position.z - this->m_FootPos.z) * (other_Position.z - this->m_FootPos.z))
		);
	return distance;
}