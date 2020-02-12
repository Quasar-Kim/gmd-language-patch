#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <detours/detours.h>
#include <nlohmann/json.hpp>
#include "internalHook.h"

#pragma comment(lib, "detours.lib")

constexpr auto COCOS_LIB_DEFAULT_PATH = L"C:/Program Files (x86)/Steam/steamapps/common/Geometry Dash/libcocos2d.dll";

using namespace std;
using json = nlohmann::json;

// Currently, hooking function 004228E0 which is responsible to color texts throws string out of range exception
// so codes associated with it are commented out

using CCLabelBMFont_setString_fn = void(__thiscall*)(void* pThis, const char* newString, bool needUpdateLabel);
using CCString_initWithFormatAndValist_fn = bool(__thiscall*)(void* pThis, const char* format, va_list ap);
// using GMDInternal_004228E0_fn = UINT(__thiscall*)(void* pThis, void* arg1, char* title, char* btn1Text, char* btn2Text, void* width, void* arg6, void* arg7, char* mainText, void* arg9, void* arg10, void* arg11, int mainTextLength, int mainTextLengthDivideTen);

// store original function for further use
CCLabelBMFont_setString_fn originalSetStringFn;
CCString_initWithFormatAndValist_fn originalInitWithFormatAndValistFn;
// GMDInternal_004228E0_fn original004228E0Fn;
//UINT __fastcall _004228E0_hookFn(void* pThis, void* _EDX, void* arg1, char* title, char* btn1Text, char* btn2Text, void* width, void* arg6, void* arg7, char* mainText, void* arg9, void* arg10, void* arg11, int mainTextLength, int mainTextLengthDivideTen)
//{
//	char newTitle[10] = "Greetings";
//	char newMainText[37] = "Do you really want to <cg>quit?</cg>";
//	return original004228E0Fn(pThis, arg1, newTitle, btn1Text, btn2Text, width, arg6, arg7, newMainText, arg9, arg10, arg11, 38, 0x2f);
// }

// this function gets called when CCLabelBMFont::setString method which is mainely used for text rendering in game gets called
// Note: When the game renders long text, it splits text into multiple lines and creates individual labels
void __fastcall setString_hookFn(void* pThis, void* _EDX, const char* labelStr, bool needUpdateLabel) {
	std::string hookedStr = labelStr;
	if (hookedStr.find("Bloodlust") != std::string::npos)
	{
		// only for test purpose
		// this code needs to be moved to dllmain
		std::ifstream translationFileStream("./translation-test.json");
		json translation;
		translationFileStream >> translation;
		std::string translatedText = translation["I have been expecting you"];
		return originalSetStringFn(pThis, translatedText.c_str(), needUpdateLabel);
	}
	return originalSetStringFn(pThis, labelStr, needUpdateLabel);
}

// this function gets called when the game creates CCString(string type used in cocos2dx game engine) with format text
// Ex. "Stereo Madness\n<cg>Total attempts:</c>%s" -> the function replaces %s with real value passed as va_list
bool __fastcall initWithFormatAndValist_hookFn(void* pThis, void* _EDX, const char* format, va_list ap)
{
	return originalInitWithFormatAndValistFn(pThis, format, ap);
}


// entry point of dll
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	LONG error;
	(void)hModule;
	(void)lpReserved;

	if (DetourIsHelperProcess()) {
		return TRUE;
	}

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DetourRestoreAfterWith();
		// hook CCLabelBMFont::create function
		HINSTANCE cocosLib = LoadLibrary(COCOS_LIB_DEFAULT_PATH);
		if (cocosLib != NULL) 
		{
			originalSetStringFn = (CCLabelBMFont_setString_fn)GetProcAddress(cocosLib, "?setString@CCLabelBMFont@cocos2d@@UAEXPBD_N@Z");
			originalInitWithFormatAndValistFn = (CCString_initWithFormatAndValist_fn)GetProcAddress(cocosLib, "?initWithFormatAndValist@CCString@cocos2d@@AAE_NPBDPAD@Z");
			// original004228E0Fn = (GMDInternal_004228E0_fn)getRVAFromOffset(0x000228E0);
			if (originalSetStringFn != NULL)
			{
				DetourTransactionBegin();
				DetourUpdateThread(GetCurrentThread());
				DetourAttach(&(PVOID&)originalSetStringFn, setString_hookFn);
				DetourAttach(&(PVOID&)originalInitWithFormatAndValistFn, initWithFormatAndValist_hookFn);
				// DetourAttach(&(PVOID&)original004228E0Fn, _004228E0_hookFn);
				error = DetourTransactionCommit();

				if (error != NO_ERROR)
				{
					MessageBoxA(0, "detour failed", "", NULL);
				}	
			}
			else
			{
				MessageBoxA(0, "Failed to load CCLAbelBMFont::create function from libcocos2d.dll", "Error", NULL);
			}
		} else
		{
			MessageBoxA(0, "Failed to load libcocos2d.dll", "Error", NULL);
		}
		break;
	}
	case DLL_PROCESS_DETACH:
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)originalSetStringFn, setString_hookFn);
		DetourDetach(&(PVOID&)originalInitWithFormatAndValistFn, initWithFormatAndValist_hookFn);
		// DetourDetach(&(PVOID&)original004228E0Fn, _004228E0_hookFn);
		DetourTransactionCommit();
		break;
	}
	return TRUE;
}

