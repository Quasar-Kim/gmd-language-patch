#include "pch.h"
#include <Windows.h>

#include "main.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		CreateThread(NULL, 0, main, NULL, 0, NULL);
		break;
	}
	}
	return true;
}