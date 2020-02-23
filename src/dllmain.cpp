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
#include <detours/detours.h>
#include <json.hpp>
// 일반 헤더 파일
#include "CCType.h"
#include "CCFunction.h"
#include "colorMarkup.h"

// NOTE: 작동에 필요한 파일들은 모두 GeometryDash.exe와 같은 폴더에 있어야 합니다
// ko-kr.json은 빌드 이벤트 스크립트에 의해 Debug 빌드될경우 64비트 컴퓨터에선 자동으로 이동됩니다
// TODO: 빌드 스크립트 32비트 자동 지원
constexpr auto COCOS_LIB_FILENAME = "libcocos2d.dll";
constexpr auto DEFAULT_TRANSLATION_FILENAME = "ko-kr.json";
constexpr auto TRANSLATION_NOT_FOUND_STR = "TRANSLATION_NOT_FOUND";

using json = nlohmann::json;

// cocos2dlib.dll에서 가져올 함수들
CCLabelBMFont_setString_fn CCLabelBMFont_setString;
CCString_initWithFormatAndValist_fn CCString_initWithFormatAndValist;
CCLabelBMFont_setAlignment_fn CCLabelBMFont_setAlignment;
CCNode_getChildByTag_fn CCNode_getChildByTag;
CCLabelBMFont_setAnchorPoint_fn CCLabelBMFont_setAnchorPoint;
CCNode_getAnchorPoint_fn CCNode_getAnchorPoint;
CCSprite_setPosition_fn CCSprite_setPosition;
CCNode_getPosition_fn CCNode_getPosition;
CCSprite_ignoreAnchorPointForPosition_fn CCSprite_ignoreAnchorPointForPosition;
CCNode_getParent_fn CCNode_getParent;
CCNode_transform_fn CCNode_transform;
CCDirector_sharedDirector_fn CCDirector_sharedDirector;
CCDirector_getRunningScene_fn CCDirector_getRunningScene;
CCNode_addChild_fn CCNode_addChild;
CCNode_removeFromParentAndCleanup_fn CCNode_removeFromParentAndCleanup;
CCNode_setZOrder_fn CCNode_setZOrder;
CCDirector_getVisibleSize_fn CCDirector_getVisibleSize;
CCNode_getContentSize_fn CCNode_getContentSize;
CCNode_setPositionXY_fn CCNode_setPositionXY;
CCSprite_addChild_fn CCSprite_addChild;
CCNode_convertToNodeSpace_fn CCNode_convertToNodeSpace;
CCLabelBMFont_limitLabelWidth_fn CCLabelBMFont_limitLabelWidth;
CCNode_convertToWorldSpace_fn CCNode_convertToWorldSpace;
CCSprite_setColor_fn CCSprite_setColor;
CCLabelBMFont_setColor_fn CCLabelBMfont_setColor;
CCNodeRGBA_setColor_fn CCNodeRGBA_setColor;
CCNodeRGBA_getColor_fn CCNodeRGBA_getColor;
CCSpriteBatchNode_addchild_fn CCSpriteBatchNode_addChild;
CCNode_setParent_fn CCNode_setParent;
CCDirector_getWinSize_fn CCDirector_getWinSize;
CCPoint_setPoint_fn CCPoint_setPoint;

json translation;
void* lastTranslatedLabel;

/* CCLabelBMFont::setString이 호출될때 대신 불리는 함수입니다
 - 색깔 표시는 무시됩니다: "<cr>Quit?</c>" -> "Quit?"
 - formatting 문자는 무시됩니다: "Total attempts: %i" -> "Total attempts: 1238283"

TODO: 번역 텍스트 색 설정 지원
*/
void* tempBackup;
void __fastcall CCLabelBMFont_setString_hookFn(void* pThis, void* _EDX, const char* labelStr, bool needUpdateLabel) {
    // 두 줄 이상 텍스트가 걸렸을때 해당 변수의 값이 true면 label을 숨깁니다
    static bool translatingMultilineStr = false;
    std::string hookedStr = labelStr;

    static std::vector<void*> multilinePartLabels;
    static std::string fullMultilineStr = "";

    // 공백 문자나 | 문자 등이 걸리는걸 방지합니다
    // 3은 휴리스틱 값입니다
    if (hookedStr.length() > 3)
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
            void* labelToRenderStr = pThis;

            if (translatingMultilineStr)
            {
                // 여러 줄 텍스트의 마지막
                multilinePartLabels.push_back(pThis);
                labelToRenderStr = multilinePartLabels[0];

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
                labelToRenderStr = pThis;
                lastTranslatedLabel = labelToRenderStr;
                translatingMultilineStr = false;
                
                // 위치 설정
                // cocos2dx의 위치 시스템에 대해서는 다음 링크를 참조해주세요:
                // http://docs.cocos.com/creator/manual/en/content-workflow/transform.html#anchor
                CCPoint position = { 0, 0 };
                CCSprite_setPosition(labelToRenderStr, position);

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

void* currentScene;
void* dialogSprite;
void* dialogParent;
void* dialogParentParent;
void* dialogParentParentParent;
void* dialogParentParentParentParent;
void __fastcall CCNode_setParent_hookFn(void* pThis, void* _EDX, void* parent)
{
    if (pThis == lastTranslatedLabel)
    {
        void* sharedDirector = CCDirector_sharedDirector();
        currentScene = CCDirector_getRunningScene(sharedDirector);
        dialogSprite = parent;
        // CCPoint centerPos = { winSize.width / 2, winSize.height / 2 };
        // CCPoint centerNodePos = CCNode_convertToNodeSpace(parent, centerPos);
    }
    else if (pThis == dialogSprite)
    {
        dialogParent = parent;
    }
    else if (pThis == dialogParent)
    {
        // maybe dialogParent is actually dialog
        dialogParentParent = parent;
        CCNode_setParent(pThis, parent);

        const CCSize& winSize = CCNode_getContentSize(currentScene);
        const CCSize& dialogParentParentSize = CCNode_getContentSize(parent);
        const CCSize& dialogParentSize = CCNode_getContentSize(dialogParent);
        const CCSize& dialogSize = CCNode_getContentSize(dialogSprite);
        
        // CCNode_setPositionXY(dialogSprite, dialogParentSize.width / 2, dialogParentSize.height / 2);
        // idea - dialogParent 사이즈 가져온다음에 /2해서 label 위치로 설정
        // idea 2 - label을 parent에서 제거하고 dialogParent에 붙이기
        CCNode_removeFromParentAndCleanup(lastTranslatedLabel, false);
        CCNode_addChild(dialogParent, lastTranslatedLabel, 100, 100);

        CCPoint centerPos = { dialogParentSize.width / 2, dialogParentSize.height / 2 };
        CCSprite_setPosition(lastTranslatedLabel, centerPos);
        CCPoint anchorPoint = { 0.5, 0.5 };
        CCLabelBMFont_setAnchorPoint(lastTranslatedLabel, anchorPoint);

        return;
    }
    return CCNode_setParent(pThis, parent);
}



// CCString::initWithFormatAndValist가 호출될때 대신 불리는 함수입니다
bool __fastcall CCString_initWithFormatAndValist_hookFn(void* pThis, void* _EDX, const char* format, va_list ap)
{
    return CCString_initWithFormatAndValist(pThis, format, ap);
}

// dll 진입점
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    LONG detourError;
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

        std::ifstream translationFileStream(DEFAULT_TRANSLATION_FILENAME);
        if (translationFileStream.is_open())
        {
            translationFileStream >> translation;

            // TODO: DLLMain에서는 LoadLibrary 호출시 에러 가능성 - 따로 init function 만들기
            HINSTANCE cocosLib = LoadLibraryA(COCOS_LIB_FILENAME);
            if (cocosLib)
            {
                // cocos2dx에서 필요한 함수들 불러오기
                CCLabelBMFont_setString = reinterpret_cast<CCLabelBMFont_setString_fn>(GetProcAddress(cocosLib, "?setString@CCLabelBMFont@cocos2d@@UAEXPBD_N@Z"));
                CCLabelBMFont_setAlignment = reinterpret_cast<CCLabelBMFont_setAlignment_fn>(GetProcAddress(cocosLib, "?setAlignment@CCLabelBMFont@cocos2d@@UAEXW4CCTextAlignment@2@@Z"));
                CCString_initWithFormatAndValist = reinterpret_cast<CCString_initWithFormatAndValist_fn>(GetProcAddress(cocosLib, "?initWithFormatAndValist@CCString@cocos2d@@AAE_NPBDPAD@Z"));
                CCNode_getChildByTag = reinterpret_cast<CCNode_getChildByTag_fn>(GetProcAddress(cocosLib, "?getChildByTag@CCNode@cocos2d@@UAEPAV12@H@Z"));
                CCLabelBMFont_setAnchorPoint = reinterpret_cast<CCLabelBMFont_setAnchorPoint_fn>(GetProcAddress(cocosLib, "?setAnchorPoint@CCLabelBMFont@cocos2d@@UAEXABVCCPoint@2@@Z"));
                CCNode_getAnchorPoint = reinterpret_cast<CCNode_getAnchorPoint_fn>(GetProcAddress(cocosLib, "?getAnchorPoint@CCNode@cocos2d@@UAEABVCCPoint@2@XZ"));
                CCSprite_setPosition = reinterpret_cast<CCSprite_setPosition_fn>(GetProcAddress(cocosLib, "?setPosition@CCSprite@cocos2d@@UAEXABVCCPoint@2@@Z"));
                CCNode_getPosition = reinterpret_cast<CCNode_getPosition_fn>(GetProcAddress(cocosLib, "?getPosition@CCNode@cocos2d@@UAEABVCCPoint@2@XZ"));
                CCSprite_ignoreAnchorPointForPosition = reinterpret_cast<CCSprite_ignoreAnchorPointForPosition_fn>(GetProcAddress(cocosLib, "?ignoreAnchorPointForPosition@CCSprite@cocos2d@@UAEX_N@Z"));
                CCNode_getParent = reinterpret_cast<CCNode_getParent_fn>(GetProcAddress(cocosLib, "?getParent@CCNode@cocos2d@@UAEPAV12@XZ"));
                CCNode_transform = reinterpret_cast<CCNode_transform_fn>(GetProcAddress(cocosLib, "?transform@CCNode@cocos2d@@QAEXXZ"));
                CCDirector_sharedDirector = reinterpret_cast<CCDirector_sharedDirector_fn>(GetProcAddress(cocosLib, "?sharedDirector@CCDirector@cocos2d@@SAPAV12@XZ"));
                CCDirector_getRunningScene = reinterpret_cast<CCDirector_getRunningScene_fn>(GetProcAddress(cocosLib, "?getRunningScene@CCDirector@cocos2d@@QAEPAVCCScene@2@XZ"));
                CCNode_addChild = reinterpret_cast<CCNode_addChild_fn>(GetProcAddress(cocosLib, "?addChild@CCNode@cocos2d@@UAEXPAV12@HH@Z"));
                CCNode_removeFromParentAndCleanup = reinterpret_cast<CCNode_removeFromParentAndCleanup_fn>(GetProcAddress(cocosLib, "?removeFromParentAndCleanup@CCNode@cocos2d@@UAEX_N@Z"));
                CCNode_setZOrder = reinterpret_cast<CCNode_setZOrder_fn>(GetProcAddress(cocosLib, "?setZOrder@CCNode@cocos2d@@UAEXH@Z"));
                CCDirector_getVisibleSize = reinterpret_cast<CCDirector_getVisibleSize_fn>(GetProcAddress(cocosLib, "?getVisibleSize@CCDirector@cocos2d@@QAE?AVCCSize@2@XZ"));
                CCNode_getContentSize = reinterpret_cast<CCNode_getContentSize_fn>(GetProcAddress(cocosLib, "?getContentSize@CCNode@cocos2d@@UBEABVCCSize@2@XZ"));
                CCNode_setPositionXY = reinterpret_cast<CCNode_setPositionXY_fn>(GetProcAddress(cocosLib, "?setPosition@CCNode@cocos2d@@UAEXMM@Z"));
                CCSprite_addChild = reinterpret_cast<CCSprite_addChild_fn>(GetProcAddress(cocosLib, "?addChild@CCSprite@cocos2d@@UAEXPAVCCNode@2@HH@Z"));
                CCNode_convertToNodeSpace = reinterpret_cast<CCNode_convertToNodeSpace_fn>(GetProcAddress(cocosLib, "?convertToNodeSpace@CCNode@cocos2d@@QAE?AVCCPoint@2@ABV32@@Z"));
                CCLabelBMFont_limitLabelWidth = reinterpret_cast<CCLabelBMFont_limitLabelWidth_fn>(GetProcAddress(cocosLib, "?limitLabelWidth@CCLabelBMFont@cocos2d@@QAEXMMM@Z"));
                CCNode_convertToWorldSpace = reinterpret_cast<CCNode_convertToWorldSpace_fn>(GetProcAddress(cocosLib, "?convertToWorldSpace@CCNode@cocos2d@@QAE?AVCCPoint@2@ABV32@@Z"));
                CCSprite_setColor = reinterpret_cast<CCSprite_setColor_fn>(GetProcAddress(cocosLib, "?setColor@CCSprite@cocos2d@@UAEXABU_ccColor3B@2@@Z"));
                CCLabelBMfont_setColor = reinterpret_cast<CCLabelBMFont_setColor_fn>(GetProcAddress(cocosLib, "?setColor@CCLabelBMFont@cocos2d@@UAEXABU_ccColor3B@2@@Z"));
                CCNodeRGBA_setColor = reinterpret_cast<CCNodeRGBA_setColor_fn>(GetProcAddress(cocosLib, "?setColor@CCNodeRGBA@cocos2d@@UAEXABU_ccColor3B@2@@Z"));
                CCNodeRGBA_getColor = reinterpret_cast<CCNodeRGBA_getColor_fn>(GetProcAddress(cocosLib, "?getColor@CCNodeRGBA@cocos2d@@UAEABU_ccColor3B@2@XZ"));
                CCSpriteBatchNode_addChild = reinterpret_cast<CCSpriteBatchNode_addchild_fn>(GetProcAddress(cocosLib, "?addChild@CCSpriteBatchNode@cocos2d@@UAEXPAVCCNode@2@HH@Z"));
                CCNode_setParent = reinterpret_cast<CCNode_setParent_fn>(GetProcAddress(cocosLib, "?setParent@CCNode@cocos2d@@UAEXPAV12@@Z"));
                CCDirector_getWinSize = reinterpret_cast<CCDirector_getWinSize_fn>(GetProcAddress(cocosLib, "?getWinSize@CCDirector@cocos2d@@QAE?AVCCSize@2@XZ"));
                CCPoint_setPoint = reinterpret_cast<CCPoint_setPoint_fn>(GetProcAddress(cocosLib, "?setPoint@CCPoint@cocos2d@@QAEXMM@Z"));

                // CCLabelBMFont::setString과 CCString::initWithFormatAndValist에 훅을 걸어 해당 함수들이 게임에서 호출될 시 
                // 이 dll의 함수들이 대신 호출되게 합니다
                DetourTransactionBegin();
                DetourUpdateThread(GetCurrentThread());
                DetourAttach(&(PVOID&)CCLabelBMFont_setString, CCLabelBMFont_setString_hookFn);
                DetourAttach(&(PVOID&)CCString_initWithFormatAndValist, CCString_initWithFormatAndValist_hookFn);
                DetourAttach(&(PVOID&)CCNode_setParent, CCNode_setParent_hookFn);
                detourError = DetourTransactionCommit();

                if (detourError != NO_ERROR)
                {
                    MessageBoxA(NULL, "Failed to attach detour to functions", "Error", NULL);
                }

            }
            else
            {
                MessageBoxA(NULL, "Failed to load libcocos2d.dll", "Error", NULL);
            }
        }
        else
        {
            MessageBoxA(NULL, "failed to load translation file", "Error", NULL);
        }
        break;
    }
    case DLL_PROCESS_DETACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)CCLabelBMFont_setString, CCLabelBMFont_setString_hookFn);
        DetourDetach(&(PVOID&)CCString_initWithFormatAndValist, CCString_initWithFormatAndValist_hookFn);
        DetourDetach(&(PVOID&)CCNode_setParent, CCNode_setParent_hookFn);
        DetourTransactionCommit();
        break;
    }
    return TRUE;
}

