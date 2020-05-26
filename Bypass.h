#pragma once
#include <Windows.h>
class Bypass
{
public:
	Bypass() {} // Constructor
	~Bypass() // Destructor
	{
		if (m_hProcess != NULL)
			CloseHandle(m_hProcess);
	}

	BOOL Attach(DWORD pid);
	BOOL Read(uintptr_t ipBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T* numberOfBytesRead = NULL);
	BOOL Write(uintptr_t ipBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T* numberOfBytesWritten = NULL);
private:
	HANDLE m_hProcess = NULL;
	
};

