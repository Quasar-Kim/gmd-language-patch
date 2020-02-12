#pragma once
#include <Windows.h>
#include <vector>

bool setInternalHook(DWORD targetFnOffset, void* hookFn, BYTE* originalInstr);

bool disableInternalHook(DWORD targetFnOffset, BYTE* originalInstr);

DWORD getRVAFromOffset(DWORD offset);