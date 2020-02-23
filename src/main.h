#pragma once

#include <Windows.h>
#include "json.hpp"

#include "CCStruct.h"

constexpr auto COCOS_LIB_FILENAME = "libcocos2d.dll";
constexpr auto DEFAULT_TRANSLATION_FILENAME = "ko-kr.json";
constexpr auto TRANSLATION_NOT_FOUND_STR = "TRANSLATION_NOT_FOUND";

using json = nlohmann::json;

DWORD WINAPI main(LPVOID lpParam);
void attachDetours();
void detachDetours();
void loadCCFn();
json loadTranslationFile(std::string translationFileName);

/* CCLabelBMFont::setString이 호출될때 대신 불리는 함수입니다
 - 색깔 표시는 무시됩니다: "<cr>Quit?</c>" -> "Quit?"
 - formatting 문자는 무시됩니다: "Total attempts: %i" -> "Total attempts: 1238283"

TODO: 번역 텍스트 색 설정 지원
*/
void __fastcall CCLabelBMFont_setString_hookFn(void* pThis, void* _EDX, const char* labelStr, bool needUpdateLabel);
void __fastcall CCSprite_setColor_hookFn(void* pThis, void* _EDX, const ccColor3B& color);
void __fastcall CCNode_setParent_hookFn(void* pThis, void* _EDX, void* parent);
bool __fastcall CCString_initWithFormatAndValist_hookFn(void* pThis, void* _EDX, const char* format, va_list ap);
