#include "pch.h"
#include <Windows.h>
// stdlib
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <cmath>
#include <functional>
// 3rd(/lib directory)
#pragma warning(push, 0)
#include <detours/detours.h>
#include <json.hpp>
#pragma warning(pop)
// header
#include "main.h"
#include "CCClass.h"
#include "CCStruct.h"
#include "CCFunction.h"
#include "CCConst.h"
#include "colorMarkup.h"

using json = nlohmann::json;

// 전역 변수
json translation;
void* labelToRenderTranslatedStr;
bool translatingMultilineStr = false;

void __fastcall CCLabelBMFont_setString_hookFn(void* pThis, void* _EDX, const char* labelStr, bool needUpdateLabel) {
	// 두 줄 이상 텍스트가 걸렸을때 해당 변수의 값이 true면 label을 숨깁니다
	std::string hookedStr = labelStr;
	static std::vector<void*> multilinePartLabels;
	static std::string fullMultilineStr = "";

	// 공백 문자나 | 문자 등이 걸리는걸 방지합니다
	if (hookedStr.length() > 0)
	{
		if (std::isspace(hookedStr.back()))
		{
			// 여러 줄 텍스트의 시작 또는 중간
			fullMultilineStr += hookedStr;
			multilinePartLabels.push_back(pThis);
			translatingMultilineStr = true;
		}
		else
		{
			std::string translatedStr;
			labelToRenderTranslatedStr = pThis;

			if (translatingMultilineStr)
			{
				// 여러 줄 텍스트의 마지막
				fullMultilineStr += hookedStr;
				translatedStr = translation.value(fullMultilineStr, TRANSLATION_NOT_FOUND_STR);
				fullMultilineStr = "";
			}
			else
			{
				// 한 줄 텍스트
				translatedStr = translation.value(hookedStr, TRANSLATION_NOT_FOUND_STR);
			}

			if (translatedStr != std::string(TRANSLATION_NOT_FOUND_STR))
			{
				if (translatingMultilineStr)
				{
					for (size_t i = 0; i < multilinePartLabels.size(); i++)
					{
						CCLabelBMFont_setString(multilinePartLabels[i], "", true);
					}
					multilinePartLabels.clear();
				}
				return CCLabelBMFont_setString(pThis, translatedStr.c_str(), true);
			}
			translatingMultilineStr = false;
			multilinePartLabels.clear();
		}
	}
	return CCLabelBMFont_setString(pThis, labelStr, needUpdateLabel);
}

void __fastcall CCSprite_setColor_hookFn(void* pThis, void* _EDX, const ccColor3B& color)
{
	return CCSprite_setColor(pThis, color);
}

void __fastcall CCNode_setParent_hookFn(void* pThis, void* _EDX, void* parent)
{
	static void* labelParentNode;
	static void* dialogSprite;
	if (translatingMultilineStr)
	{
		if (pThis == labelToRenderTranslatedStr)
		{
			labelParentNode = parent;
		}
		else if (pThis == labelParentNode)
		{
			dialogSprite = parent;
		}
		else if (pThis == dialogSprite)
		{
			// use removeFromParent instead?
			CCNode_removeFromParentAndCleanup(labelToRenderTranslatedStr, false);
			CCNode_addChild(dialogSprite, labelToRenderTranslatedStr, 100, 100);

			const CCSize& dialogParentSize = CCNode_getContentSize(dialogSprite);
			CCPoint centerPos = { dialogParentSize.width / 2, dialogParentSize.height / 2 };
			CCPoint anchorPoint = { 0.5, 0.5 };

			CCSprite_setPosition(labelToRenderTranslatedStr, centerPos);
			CCLabelBMFont_setAnchorPoint(labelToRenderTranslatedStr, anchorPoint);
			CCLabelBMFont_setAlignment(labelToRenderTranslatedStr, CC_TEXT_ALIGNMENT_CENTER);

			translatingMultilineStr = false;
			labelParentNode = nullptr;
			dialogSprite = nullptr;
		}
	}
	return CCNode_setParent(pThis, parent);
}

bool __fastcall CCString_initWithFormatAndValist_hookFn(void* pThis, void* _EDX, const char* format, va_list ap)
{
	return CCString_initWithFormatAndValist(pThis, format, ap);
}

#pragma warning(push)
#pragma warning(disable:4326)
DWORD WINAPI main(LPVOID lpParam)
{
	try
	{
		translation = loadTranslationFile(DEFAULT_TRANSLATION_FILENAME);
		loadCCFn();
		attachDetours();
	}
	catch (std::runtime_error & e)
	{
		detachDetours();
		std::string errReportMsg = "Unexcepted error has occured in languagePatch.dll: ";
		errReportMsg.append(e.what());
		MessageBoxA(NULL, errReportMsg.c_str(), "Language Patch Error", MB_ICONERROR);
		return static_cast<DWORD>(1);
	}
	return static_cast<DWORD>(0);
}
#pragma warning(pop)

json loadTranslationFile(std::string translationFileName)
{
	std::ifstream translationFileStream(translationFileName);
	if (translationFileStream.is_open())
	{
		json translationObj;
		translationFileStream >> translationObj;
		return translationObj;
	}
	else
	{
		throw std::runtime_error("Failed to load tranlsation file");
	}
}

void attachDetours()
{
	LONG detourError;

	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)CCLabelBMFont_setString, CCLabelBMFont_setString_hookFn);
	DetourAttach(&(PVOID&)CCString_initWithFormatAndValist, CCString_initWithFormatAndValist_hookFn);
	DetourAttach(&(PVOID&)CCNode_setParent, CCNode_setParent_hookFn);
	detourError = DetourTransactionCommit();

	if (detourError != NO_ERROR)
	{
		throw std::runtime_error("Failed to attach detours");
	}
}

void detachDetours()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)CCLabelBMFont_setString, CCLabelBMFont_setString_hookFn);
	DetourDetach(&(PVOID&)CCString_initWithFormatAndValist, CCString_initWithFormatAndValist_hookFn);
	DetourDetach(&(PVOID&)CCNode_setParent, CCNode_setParent_hookFn);
	DetourTransactionCommit();
}



