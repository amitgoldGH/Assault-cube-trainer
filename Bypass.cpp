#include "Bypass.h"

BOOL Bypass::Attach(DWORD pid)
{
	if (m_hProcess != NULL) // Check if a handle was already set before attaching
		CloseHandle(m_hProcess);

	m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (m_hProcess != NULL) // Check if we managed to attach
		return TRUE;
	else
		return FALSE;
}

BOOL Bypass::Read
(
	uintptr_t ipBaseAddress,
	LPVOID lpBuffer,
	SIZE_T nSize,
	SIZE_T* numberOfBytesRead
)
{
	if (m_hProcess == NULL)
		return FALSE; // No handle to read with

	return ReadProcessMemory(m_hProcess, (LPCVOID)ipBaseAddress,
		lpBuffer, nSize, numberOfBytesRead);
}

BOOL Bypass::Write
(
	uintptr_t ipBaseAddress,
	LPVOID lpBuffer,
	SIZE_T nSize,
	SIZE_T* numberOfBytesWritten
)
{
	if (m_hProcess == NULL)
		return FALSE; // No handle to read with

	return WriteProcessMemory(m_hProcess, (LPVOID)ipBaseAddress,
		lpBuffer, nSize, numberOfBytesWritten);
}