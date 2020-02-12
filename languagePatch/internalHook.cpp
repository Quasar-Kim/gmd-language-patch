#include "pch.h"
#include <Windows.h>
#include <vector>
#include "internalHook.h"

using namespace std;

// setInternalHook and disableInternalHook is currently not in use
// in favor of detours library

// replace instruction at given offset to jump to hookFn
bool setInternalHook(DWORD targetFnOffset, void* hookFn, BYTE* originalInstr)
{
	HANDLE gameModuleHandle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_VM_OPERATION, FALSE, GetCurrentProcessId());
	if (gameModuleHandle != NULL)
	{
		// make exe memory writable
		DWORD oldProtect;
		DWORD targetFnRVA = getRVAFromOffset(targetFnOffset);
		if (VirtualProtect((LPVOID)targetFnRVA, 6, PAGE_EXECUTE_READWRITE, &oldProtect) == TRUE)
		{
			// store original instruction
			if (ReadProcessMemory(gameModuleHandle, (LPCVOID)targetFnRVA, &originalInstr, 6, NULL) == TRUE)
			{
				// prepare JMP instruction for hook
				// interpreted as JMP 0x00HEXHEX
				BYTE jmpInstr[6] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0xC3 };
				// 0xE9 JMP is relative, so calculate hookFn's relative
				DWORD hookFnRelativeAddr = ((DWORD)hookFn - targetFnRVA - 5);
				// copy calculated relative addr into JMP instruction
				memcpy(&jmpInstr[1], &hookFnRelativeAddr, 4);
				// write memory
				if (WriteProcessMemory(gameModuleHandle, (LPVOID)targetFnRVA, &jmpInstr, 6, 0) == TRUE)
				{
					if (VirtualProtect((LPVOID)targetFnRVA, 6, oldProtect, &oldProtect))
					{
						return true;
					}
				}
			}
		}

	}
	else
	{
		MessageBoxA(0, "Failed to get handle of GeometryDash.exe", "Error", NULL);
	}
	return false;
}

bool disableInternalHook(DWORD targetFnOffset, BYTE* originalInstr) {
	HANDLE gameModuleHandle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, GetCurrentProcessId());
	if (gameModuleHandle != NULL)
	{
		// make exe memory writable
		DWORD oldProtect;
		DWORD targetFnRVA = getRVAFromOffset(targetFnOffset);
		if (VirtualProtect((LPVOID)targetFnRVA, 6, PAGE_EXECUTE_READWRITE, &oldProtect) == TRUE)
		{
			if (WriteProcessMemory(gameModuleHandle, (LPVOID)targetFnRVA, &originalInstr, 6, 0) == TRUE)
			{
				if (VirtualProtect((LPVOID)targetFnRVA, 6, oldProtect, &oldProtect))
				{
					return true;
				}
			}
		}

	}
	else
	{
		MessageBoxA(0, "Failed to get handle of GeometryDash.exe", "Error", NULL);
	}
	return false;
}

// GeometryDash.exe uses ASLR(Address Space Load Randamization)
// therefore in order to hook functions inside exe file
// approrpiate RVA(Relative Virtual Address) needs to be calculated

// offset parameter is memory address(can be found in tools like IDA or Ghidra) - 0x400000(image base declared in exe file)
// Ex. in order to retrieve RVA of function which IDA shows to be located at 0x42A5A0
// set offset parameter to 0x2A5A0
DWORD getRVAFromOffset(DWORD offset)
{
	// get current imageBase
	DWORD imageBase = (DWORD)GetModuleHandle(NULL);
	return imageBase + offset;
}