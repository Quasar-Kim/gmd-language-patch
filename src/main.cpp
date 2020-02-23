#include "pch.h"
#include <Windows.h>
// Standard Library
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <cmath>
#include <functional>
// Third Party Library(lib/)
#pragma warning(push, 0)
#include <detours/detours.h>
#include <json.hpp>
#pragma warning(pop)
// 일반 헤더 파일
#include "main.h"
#include "CCClass.h"
#include "CCStruct.h"
#include "CCFunction.h"
#include "CCConst.h"
#include "colorMarkup.h"

using json = nlohmann::json;

// cocos2dlib.dll에서 가져올 함수들
// TODO: 이름 이용해서 자동으로 불러올수 있게 개선
CCLabelBMFont_setString_fn CCLabelBMFont_setString;
CCLabelBMFont_setAlignment_fn CCLabelBMFont_setAlignment;
CCLabelBMFont_setAnchorPoint_fn CCLabelBMFont_setAnchorPoint;
CCLabelBMFont_setColor_fn CCLabelBMfont_setColor;

CCString_initWithFormatAndValist_fn CCString_initWithFormatAndValist;

CCNode_getChildByTag_fn CCNode_getChildByTag;
CCNode_addChild_fn CCNode_addChild;
CCNode_removeFromParentAndCleanup_fn CCNode_removeFromParentAndCleanup;
CCNode_getContentSize_fn CCNode_getContentSize;
CCNode_setParent_fn CCNode_setParent;

CCSprite_setPosition_fn CCSprite_setPosition;
CCSprite_addChild_fn CCSprite_addChild;
CCSprite_setColor_fn CCSprite_setColor;

CCDirector_sharedDirector_fn CCDirector_sharedDirector;
CCDirector_getRunningScene_fn CCDirector_getRunningScene;
CCDirector_getVisibleSize_fn CCDirector_getVisibleSize;

CCNodeRGBA_setColor_fn CCNodeRGBA_setColor;
CCNodeRGBA_getColor_fn CCNodeRGBA_getColor;

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

void loadCCFn()
{
	HINSTANCE cocosLib = LoadLibraryA(COCOS_LIB_FILENAME);
	if (cocosLib)
	{
		// cocos2dx에서 필요한 함수들 불러오기
		// TODO: non-nil guarantee
		CCLabelBMFont_setString = reinterpret_cast<CCLabelBMFont_setString_fn>(GetProcAddress(cocosLib, "?setString@CCLabelBMFont@cocos2d@@UAEXPBD_N@Z"));
		CCLabelBMFont_setAlignment = reinterpret_cast<CCLabelBMFont_setAlignment_fn>(GetProcAddress(cocosLib, "?setAlignment@CCLabelBMFont@cocos2d@@UAEXW4CCTextAlignment@2@@Z"));
		CCString_initWithFormatAndValist = reinterpret_cast<CCString_initWithFormatAndValist_fn>(GetProcAddress(cocosLib, "?initWithFormatAndValist@CCString@cocos2d@@AAE_NPBDPAD@Z"));
		CCNode_getChildByTag = reinterpret_cast<CCNode_getChildByTag_fn>(GetProcAddress(cocosLib, "?getChildByTag@CCNode@cocos2d@@UAEPAV12@H@Z"));
		CCLabelBMFont_setAnchorPoint = reinterpret_cast<CCLabelBMFont_setAnchorPoint_fn>(GetProcAddress(cocosLib, "?setAnchorPoint@CCLabelBMFont@cocos2d@@UAEXABVCCPoint@2@@Z"));
		CCSprite_setPosition = reinterpret_cast<CCSprite_setPosition_fn>(GetProcAddress(cocosLib, "?setPosition@CCSprite@cocos2d@@UAEXABVCCPoint@2@@Z"));
		CCDirector_sharedDirector = reinterpret_cast<CCDirector_sharedDirector_fn>(GetProcAddress(cocosLib, "?sharedDirector@CCDirector@cocos2d@@SAPAV12@XZ"));
		CCDirector_getRunningScene = reinterpret_cast<CCDirector_getRunningScene_fn>(GetProcAddress(cocosLib, "?getRunningScene@CCDirector@cocos2d@@QAEPAVCCScene@2@XZ"));
		CCNode_addChild = reinterpret_cast<CCNode_addChild_fn>(GetProcAddress(cocosLib, "?addChild@CCNode@cocos2d@@UAEXPAV12@HH@Z"));
		CCNode_removeFromParentAndCleanup = reinterpret_cast<CCNode_removeFromParentAndCleanup_fn>(GetProcAddress(cocosLib, "?removeFromParentAndCleanup@CCNode@cocos2d@@UAEX_N@Z"));
		CCDirector_getVisibleSize = reinterpret_cast<CCDirector_getVisibleSize_fn>(GetProcAddress(cocosLib, "?getVisibleSize@CCDirector@cocos2d@@QAE?AVCCSize@2@XZ"));
		CCNode_getContentSize = reinterpret_cast<CCNode_getContentSize_fn>(GetProcAddress(cocosLib, "?getContentSize@CCNode@cocos2d@@UBEABVCCSize@2@XZ"));
		CCSprite_addChild = reinterpret_cast<CCSprite_addChild_fn>(GetProcAddress(cocosLib, "?addChild@CCSprite@cocos2d@@UAEXPAVCCNode@2@HH@Z"));
		CCSprite_setColor = reinterpret_cast<CCSprite_setColor_fn>(GetProcAddress(cocosLib, "?setColor@CCSprite@cocos2d@@UAEXABU_ccColor3B@2@@Z"));
		CCLabelBMfont_setColor = reinterpret_cast<CCLabelBMFont_setColor_fn>(GetProcAddress(cocosLib, "?setColor@CCLabelBMFont@cocos2d@@UAEXABU_ccColor3B@2@@Z"));
		CCNodeRGBA_setColor = reinterpret_cast<CCNodeRGBA_setColor_fn>(GetProcAddress(cocosLib, "?setColor@CCNodeRGBA@cocos2d@@UAEXABU_ccColor3B@2@@Z"));
		CCNodeRGBA_getColor = reinterpret_cast<CCNodeRGBA_getColor_fn>(GetProcAddress(cocosLib, "?getColor@CCNodeRGBA@cocos2d@@UAEABU_ccColor3B@2@XZ"));
		CCNode_setParent = reinterpret_cast<CCNode_setParent_fn>(GetProcAddress(cocosLib, "?setParent@CCNode@cocos2d@@UAEXPAV12@@Z"));
	}
	else
	{
		throw std::runtime_error("Failed to load function from cocos2dlib.dll");
	}
}

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



