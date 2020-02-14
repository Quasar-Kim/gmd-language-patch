#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <cmath>
#include <detours/detours.h>
#include <nlohmann/json.hpp>
#include "dllmain.h"
#include "internalHook.h"

#pragma comment(lib, "detours.lib")

// TODO: 32비트 환경에서는 다른 경로 지원 필요
constexpr auto COCOS_LIB_DEFAULT_PATH = "C:/Program Files (x86)/Steam/steamapps/common/Geometry Dash/libcocos2d.dll";
constexpr auto TRANSLATION_NOT_FOUND = "TRANSLATION_NOT_FOUND";

using namespace std;
using json = nlohmann::json;

// TODO: 안쓰는 함수 정리
using CCLabelBMFont_setString_fn = void(__thiscall*)(void* pThis, const char* newString, bool needUpdateLabel);
using CCString_initWithFormatAndValist_fn = bool(__thiscall*)(void* pThis, const char* format, va_list ap);
using CCLabelBMFont_setAlignment_fn = void(__thiscall*)(void* pThis, int alignment);
using CCNodeRGBA_getColor_fn = const void* (__thiscall*)(void* pThis);
using CCNode_getChildByTag_fn = void* (__thiscall*)(void* pThis, int tag);
using CCLabelBMFont_setAnchorPoint_fn = void(__thiscall*)(void* pThis, void* anchorPoint);
using CCNode_getAnchorPoint_fn = void* (__thiscall*)(void* pThis);
using CCPoint_setPoint_fn = void(__thiscall*)(void* pThis, float x, float y);
using CCSprite_setPosition_fn = void(__thiscall*)(void* pThis, void* position);
using CCNode_getPosition_fn = void*(__thiscall*)(void* pThis);
using CCSprite_ignoreAnchorPointForPosition_fn = void(__thiscall*)(void* pThis, bool ignore);
using CCNode_getParent_fn = void* (__thiscall*)(void* pThis);
using CCNode_setPositionY_fn = void(__thiscall*)(void* pThis, float y);
using CCNode_getPositionXY_fn = void(__thiscall*)(void* pThis, float* x, float* y);
using CCNode_transform_fn = void* (__thiscall*)(void* pThis);

CCLabelBMFont_setString_fn CCLabelBMFont_setString;
CCString_initWithFormatAndValist_fn CCString_initWithFormatAndValist;
CCLabelBMFont_setAlignment_fn CCLabelBMFont_setAlignment;
CCNodeRGBA_getColor_fn CCNodeRGBA_getColor;
CCNode_getChildByTag_fn CCNode_getChildByTag;
CCLabelBMFont_setAnchorPoint_fn CCLabelBMFont_setAnchorPoint;
CCNode_getAnchorPoint_fn CCNode_getAnchorPoint;
CCPoint_setPoint_fn CCPoint_setPoint;
CCSprite_setPosition_fn CCSprite_setPosition;
CCNode_getPosition_fn CCNode_getPosition;
CCSprite_ignoreAnchorPointForPosition_fn CCSprite_ignoreAnchorPointForPosition;
CCNode_getParent_fn CCNode_getParent;
CCNode_setPositionY_fn CCNode_setPositionY;
CCNode_getPositionXY_fn CCNode_getPositionXY;
CCNode_transform_fn CCNode_transform;

json translation;

/* CCLabelBMFont::setString이 호출될때 대신 불리는 함수입니다
Note: 두 줄 이상의 텍스트의 경우 GMD는 cocos2dx의 자동 개행 기능을 이용하지 않고 줄마다 서도 다른 CCLabelBMFont를 생성합니다.
    그렇기 때문에 각각의 줄만을 보고는 무슨 텍스트를 출력 중인지 알 수 없는 경우가 생길 수 있고, 따라서
    한 줄마다 그에 해당하는 번역을 출력하면 잘못된 번역을 보여줄 가능성이 있습니다.

        Ex) This trigger moves...
            mode. <-- 이 단어 하나만 보고는 무슨말을 하고 있는 것인지 알 수 없습니다.

    따라서 텍스트의 첫 줄을 보고 번역된 텍스트 전체를 하나의 CCLabelBMFont안에 렌더링 한 후(거의 대부분의 텍스트는 첫 줄만으로 구별할 수 있습니다)
    이어지는 CCLabelBMFont들은 보이지 않게 설정해야 합니다.
TODO: 색깔 설정 지원
*/
void __fastcall CCLabelBMFont_setString_hookFn(void* pThis, void* _EDX, const char* labelStr, bool needUpdateLabel) {
    // 두 줄 이상 텍스트가 걸렸을때 해당 변수의 값이 true면 label을 숨깁니다
    static bool translatingMultilineStr = false;
    std::string hookedStr = labelStr;

    static vector<void*> multilinePartLabels;
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
            double labelToRenderstrIndex = 0;

            if (translatingMultilineStr)
            {
                // 여러 줄 텍스트의 마지막
                multilinePartLabels.push_back(pThis);
                labelToRenderstrIndex = std::trunc(multilinePartLabels.size() / 2);
                labelToRenderStr = multilinePartLabels.at(labelToRenderstrIndex);

                fullMultilineStr += hookedStr;
                translatedStr = translation.value(fullMultilineStr, TRANSLATION_NOT_FOUND);
                fullMultilineStr = "";
            }
            else
            {
                // 한 줄 텍스트
                translatedStr = translation.value(hookedStr, TRANSLATION_NOT_FOUND);
            }

            if (translatedStr != std::string(TRANSLATION_NOT_FOUND))
            {
                if (translatingMultilineStr)
                {
                    // TODO: iterator과 labelToRenderStr 포인터 비교
                    for (int i = 0; i < multilinePartLabels.size(); i++)
                    {
                        if (i == labelToRenderstrIndex)
                            continue;
                        // TODO: 폰트에 없는 문자를 렌더링하는 대신 setVisible() 이용
                        CCLabelBMFont_setString(multilinePartLabels.at(i), "♬", true);
                    }
                    multilinePartLabels.clear();
                }
                // 가운데 정렬
                CCLabelBMFont_setAlignment(labelToRenderStr, 1);

                // anchorpoint 설정
                // 일부 텍스트들에 대해서만 실험되었습니다
                void* anchorPoint = CCNode_getAnchorPoint(labelToRenderStr);
                // 휴리스틱 값, 잘못 작동할 확률이 높습니다
                CCPoint_setPoint(anchorPoint, 0.17, 0.15);
                CCLabelBMFont_setAnchorPoint(labelToRenderStr, anchorPoint);

                translatingMultilineStr = false;
                return CCLabelBMFont_setString(labelToRenderStr, translatedStr.c_str(), needUpdateLabel);
            }
            translatingMultilineStr = false;
            multilinePartLabels.clear();
        }
    }
    return CCLabelBMFont_setString(pThis, labelStr, needUpdateLabel);
}

// CCString::initWithFormatAndValist가 호출될때 대신 불리는 함수입니다
bool __fastcall CCString_initWithFormatAndValist_hookFn(void* pThis, void* _EDX, const char* format, va_list ap)
{
    return CCString_initWithFormatAndValist(pThis, format, ap);
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

        std::ifstream translationFileStream("./ko-kr.json");
        if (translationFileStream.is_open())
        {
            translationFileStream >> translation;

            HINSTANCE cocosLib = LoadLibraryA(COCOS_LIB_DEFAULT_PATH);
            if (cocosLib != NULL)
            {
                // cocos2dx에서 필요한 함수들 불러오기
                // TODO: 비주얼 스튜디오의 자동 불러오기 기능 이용?
                CCLabelBMFont_setString = (CCLabelBMFont_setString_fn)GetProcAddress(cocosLib, "?setString@CCLabelBMFont@cocos2d@@UAEXPBD_N@Z");
                CCLabelBMFont_setAlignment = (CCLabelBMFont_setAlignment_fn)GetProcAddress(cocosLib, "?setAlignment@CCLabelBMFont@cocos2d@@UAEXW4CCTextAlignment@2@@Z");
                CCString_initWithFormatAndValist = (CCString_initWithFormatAndValist_fn)GetProcAddress(cocosLib, "?initWithFormatAndValist@CCString@cocos2d@@AAE_NPBDPAD@Z");
                CCNodeRGBA_getColor = (CCNodeRGBA_getColor_fn)GetProcAddress(cocosLib, "?getColor@CCNodeRGBA@cocos2d@@UAEABU_ccColor3B@2@XZ");
                CCNode_getChildByTag = (CCNode_getChildByTag_fn)GetProcAddress(cocosLib, "?getChildByTag@CCNode@cocos2d@@UAEPAV12@H@Z");
                CCLabelBMFont_setAnchorPoint = (CCLabelBMFont_setAnchorPoint_fn)GetProcAddress(cocosLib, "?setAnchorPoint@CCLabelBMFont@cocos2d@@UAEXABVCCPoint@2@@Z");
                CCNode_getAnchorPoint = (CCNode_getAnchorPoint_fn)GetProcAddress(cocosLib, "?getAnchorPoint@CCNode@cocos2d@@UAEABVCCPoint@2@XZ");
                CCPoint_setPoint = (CCPoint_setPoint_fn)GetProcAddress(cocosLib, "?setPoint@CCPoint@cocos2d@@QAEXMM@Z");
                CCSprite_setPosition = (CCSprite_setPosition_fn)GetProcAddress(cocosLib, "?setPosition@CCSprite@cocos2d@@UAEXABVCCPoint@2@@Z");
                CCNode_getPosition = (CCNode_getPosition_fn)GetProcAddress(cocosLib, "?getPosition@CCNode@cocos2d@@UAEABVCCPoint@2@XZ");
                CCSprite_ignoreAnchorPointForPosition = (CCSprite_ignoreAnchorPointForPosition_fn)GetProcAddress(cocosLib, "?ignoreAnchorPointForPosition@CCSprite@cocos2d@@UAEX_N@Z");
                CCNode_getParent = (CCNode_getParent_fn)GetProcAddress(cocosLib, "?getParent@CCNode@cocos2d@@UAEPAV12@XZ");
                CCNode_setPositionY = (CCNode_setPositionY_fn)GetProcAddress(cocosLib, "?setPositionY@CCNode@cocos2d@@UAEXM@Z");
                CCNode_getPositionXY = (CCNode_getPositionXY_fn)GetProcAddress(cocosLib, "?getPosition@CCNode@cocos2d@@UAEXPAM0@Z");
                CCNode_transform = (CCNode_transform_fn)GetProcAddress(cocosLib, "?transform@CCNode@cocos2d@@QAEXXZ");

                // CCLabelBMFont::setString과 CCString::initWithFormatAndValist에 훅을 걸어 해당 함수들이 게임에서 호출될 시 
                // 이 dll의 함수들이 대신 호출되게 합니다
                DetourTransactionBegin();
                DetourUpdateThread(GetCurrentThread());
                DetourAttach(&(PVOID&)CCLabelBMFont_setString, CCLabelBMFont_setString_hookFn);
                DetourAttach(&(PVOID&)CCString_initWithFormatAndValist, CCString_initWithFormatAndValist_hookFn);
                error = DetourTransactionCommit();

                if (error != NO_ERROR)
                {
                    MessageBoxA(0, "detour failed", "Error", NULL);
                }

            }
            else
            {
                MessageBoxA(0, "Failed to load libcocos2d.dll", "Error", NULL);
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
        DetourTransactionCommit();
        break;
    }
    return TRUE;
}

