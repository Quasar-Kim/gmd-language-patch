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
bool translatingStr = false;
int calledTime = 0;
std::vector<StrColorInfo> colorInfo;

void __fastcall CCLabelBMFont_setString_hookFn(void* pThis, void* _EDX, const char* labelStr, bool needUpdateLabel) {
	// 두 줄 이상 텍스트가 걸렸을때 해당 변수의 값이 true면 label을 숨깁니다
	std::string hookedStr = labelStr;
	static std::vector<void*> multilinePartLabels;
	static std::string fullMultilineStr = "";
	static bool translatingMultilineStr = false;

	// 공백 문자나 | 문자 등이 걸리는걸 방지합니다
	if (hookedStr.length() > 1)
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
				translatingMultilineStr = false;
				translatingStr = true;
				labelToRenderTranslatedStr = pThis;

				colorInfo = parseColorMarkup(translatedStr);
				translatedStr = removeColorMarkup(translatedStr);

				return CCLabelBMFont_setString(pThis, translatedStr.c_str(), true);
			}
			translatingMultilineStr = false;
			multilinePartLabels.clear();
		}
	}
	return CCLabelBMFont_setString(pThis, labelStr, needUpdateLabel);
}

void __fastcall CCNode_setParent_hookFn(void* pThis, void* _EDX, void* parent)
{
	static void* labelParentNode;
	static void* dialogSprite;
	if (translatingStr)
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
			// TODO: removeFromParent 대신 사용?
			CCNode_removeFromParentAndCleanup(labelToRenderTranslatedStr, false);
			CCNode_addChild(dialogSprite, labelToRenderTranslatedStr, 100, 100);

			// label 속성 설정
			const CCSize& dialogParentSize = CCNode_getContentSize(dialogSprite);
			CCPoint centerPos = { dialogParentSize.width / 2, dialogParentSize.height / 2 };
			CCPoint anchorPoint = { 0.5, 0.5 };
			
			CCSprite_setPosition(labelToRenderTranslatedStr, centerPos);
			CCLabelBMFont_setAnchorPoint(labelToRenderTranslatedStr, anchorPoint);
			CCLabelBMFont_setAlignment(labelToRenderTranslatedStr, CC_TEXT_ALIGNMENT_CENTER);

			translatingStr = false;
			labelParentNode = nullptr;
			dialogSprite = nullptr;
			calledTime = 0;
			colorInfo.clear();
		}
	}
	return CCNode_setParent(pThis, parent);
}

void __fastcall CCSprite_setColor_hookFn(void* pThis, void* _EDX, const ccColor3B& color)
{
	if (translatingStr)
	{
		return;
	}
	return CCSprite_setColor(pThis, color);
}

bool __fastcall CCString_initWithFormatAndValist_hookFn(void* pThis, void* _EDX, const char* format, va_list ap)
{
	return CCString_initWithFormatAndValist(pThis, format, ap);
}

void __fastcall CCSprite_setOpacityModifyRGB_hookFn(void* pThis, void* _EDX, bool modify)
{
	if (translatingStr)
	{
		if (calledTime % 2 == 0)
		{
			int charIndex = calledTime / 2;
			for (const StrColorInfo& strColorInfo : colorInfo)
			{
				if (charIndex <= strColorInfo.endPos && charIndex >= strColorInfo.startPos)
				{
					CCSprite_setColor(pThis, strColorInfo.color);
					break;
				}
			}
		}
		calledTime++;
	}
	return CCSprite_setOpacityModifyRGB(pThis, modify);
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
		return 1;
	}
	return 0;
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
	DetourAttach(&(PVOID&)CCSprite_setColor, CCSprite_setColor_hookFn);
	DetourAttach(&(PVOID&)CCSprite_setOpacityModifyRGB, CCSprite_setOpacityModifyRGB_hookFn);
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
	DetourDetach(&(PVOID&)CCSprite_setColor, CCSprite_setColor_hookFn);
	DetourDetach(&(PVOID&)CCSprite_setOpacityModifyRGB, CCSprite_setOpacityModifyRGB_hookFn);
	DetourTransactionCommit();
}



