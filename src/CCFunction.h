#pragma once
#include <Windows.h>

#include <functional>

#include "CCClass.h"
#include "CCStruct.h"

// cocos2dlib.dll에서 불러올 함수들
// TODO: boost.dll 사용?
/* NOTE: 새로운 함수를 불러올 시
    - 함수 포인터 선언 추가(CCFunction.h)
    - extern 함수 전역 변수 추가(CCFunction.h)
    - 함수 전역 변수 초기화(CCFunction.cpp)
    - 함수 불러오기(CCFunction.cpp loadCCFn)
*/

// 함수 포인터 선언 ------------------------------------------------------
using CCLabelBMFont_setString_fn = void(__thiscall*)(void* pThis, const char* newString, bool needUpdateLabel);
using CCLabelBMFont_limitLabelWidth_fn = void(__thiscall*)(void* pThis, float pa, float pb, float pc);
using CCLabelBMFont_setAlignment_fn = void(__thiscall*)(void* pThis, int alignment);
using CCLabelBMFont_setAnchorPoint_fn = void(__thiscall*)(void* pThis, const CCPoint & anchorPoint);
using CCLabelBMFont_setColor_fn = void(__thiscall*)(void* pThis, const CCColor3B & color);
using CCLabelBMFont_create_fn = void*(*)(const char* str, const char* fntFile);
using CCLabelBMFont_setCascadeColorEnabled_fn = void(__thiscall*)(void* pThis, bool cascadeColorEnabled);

using CCSprite_setPosition_fn = void(__thiscall*)(void* pThis, const CCPoint & position);
using CCSprite_ignoreAnchorPointForPosition_fn = void(__thiscall*)(void* pThis, bool ignore);
using CCSprite_addChild_fn = void(__thiscall*)(void* pThis, void* child, int zOrder, int tag);
using CCSprite_setColor_fn = void(__thiscall*)(void* pThis, const CCColor3B & color);
using CCSprite_setBatchNode_fn = void(__thiscall*)(void* pThis, void* pobSpriteBatchNode);
using CCSprite_initWithTexture_fn = void(__thiscall*)(void* pThis, void* pTexture, void* rect, bool rotated);
using CCSprite_setOpacityModifyRGB_fn = void(__thiscall*)(void* pThis, bool modify);

using CCNode_getChildByTag_fn = void* (__thiscall*)(void* pThis, int tag);
using CCNode_getAnchorPoint_fn = void* (__thiscall*)(void* pThis);
using CCNode_getPosition_fn = void* (__thiscall*)(void* pThis);
using CCNode_getParent_fn = void* (__thiscall*)(void* pThis);
using CCNode_transform_fn = void* (__thiscall*)(void* pThis);
using CCNode_addChild_fn = void(__thiscall*)(void* pThis, void* child, int zOrder, int tag);
using CCNode_removeFromParentAndCleanup_fn = void(__thiscall*)(void* pThis, bool cleanup);
using CCNode_setZOrder_fn = void(__thiscall*)(void* pThis, int zOrder);
using CCNode_getContentSize_fn = const CCSize & (__thiscall*)(void* pThis);
using CCNode_setPositionXY_fn = void(__thiscall*)(void* pThis, float x, float y);
using CCNode_convertToNodeSpace_fn = CCPoint(__thiscall*)(void* pThis, void* worldPoint);
using CCNode_convertToWorldSpace_fn = CCPoint(__thiscall*)(void* pThis, const CCPoint & nodePoint);
using CCNode_setParent_fn = void(__thiscall*)(void* pThis, void* parent);
using CCSprite_setVisible_fn = void(__thiscall*)(void* pThis, bool visible);
using CCNode_getChildren_fn = void* (__thiscall*)(void* pThis);

using CCObject_getTag_fn = int(__thiscall*)(void* pThis);

using CCArray_randomObject_fn = void* (__thiscall*)(void* pThis);
using CCArray_indexOf_fn = unsigned int(__thiscall*)(void* pThis, void* object);

using CCDirector_getRunningScene_fn = void* (__thiscall*)(void* pThis);
using CCDirector_getVisibleSize_fn = void* (__thiscall*)(void* pThis);
using CCDirector_sharedDirector_fn = void* (*)();
using CCDirector_getWinSize_fn = CCSize (__thiscall*)(void* pThis);

using CCString_initWithFormatAndValist_fn = bool(__thiscall*)(void* pThis, const char* format, va_list ap);

using CCNodeRGBA_setColor_fn = void(__thiscall*)(void* pThis, const CCColor3B & color);
using CCNodeRGBA_getColor_fn = CCColor3B & (__thiscall*)(void* pThis);
using CCNodeRGBA_getDisplayedColor_fn = const CCColor3B & (__thiscall*)(void* pThis);
using CCNodeRGBA_getOpacity_fn = unsigned char(__thiscall*)(void* pThis);

using CCSpriteBatchNode_addchild_fn = void(__thiscall*)(void* pThis, void* child, int zOrder, int tag);

using CCPoint_setPoint_fn = void(__thiscall*)(void* pThis, float x, float y);

// 함수 전역 변수 ----------------------------------------------------
// loadCCFn에 의해서 적절한 함수로 설정됩니다
extern CCLabelBMFont_setString_fn CCLabelBMFont_setString;
extern CCLabelBMFont_setAlignment_fn CCLabelBMFont_setAlignment;
extern CCLabelBMFont_setAnchorPoint_fn CCLabelBMFont_setAnchorPoint;
extern CCLabelBMFont_setColor_fn CCLabelBMfont_setColor;

extern CCString_initWithFormatAndValist_fn CCString_initWithFormatAndValist;

extern CCNode_getChildByTag_fn CCNode_getChildByTag;
extern CCNode_addChild_fn CCNode_addChild;
extern CCNode_removeFromParentAndCleanup_fn CCNode_removeFromParentAndCleanup;
extern CCNode_getContentSize_fn CCNode_getContentSize;
extern CCNode_setParent_fn CCNode_setParent;
extern CCNode_getChildren_fn CCNode_getChildren;
extern CCNode_getParent_fn CCNode_getParent;

extern CCObject_getTag_fn CCObject_getTag;

extern CCArray_randomObject_fn CCArray_randomObject;
extern CCArray_indexOf_fn CCArray_indexOf;

extern CCSprite_setPosition_fn CCSprite_setPosition;
extern CCSprite_addChild_fn CCSprite_addChild;
extern CCSprite_setColor_fn CCSprite_setColor;
extern CCSprite_setVisible_fn CCSprite_setVisible;
extern CCSprite_setBatchNode_fn CCSprite_setBatchNode;
extern CCSprite_initWithTexture_fn CCSprite_initWithTexture;
extern CCSprite_setOpacityModifyRGB_fn CCSprite_setOpacityModifyRGB;

extern CCDirector_sharedDirector_fn CCDirector_sharedDirector;
extern CCDirector_getRunningScene_fn CCDirector_getRunningScene;
extern CCDirector_getVisibleSize_fn CCDirector_getVisibleSize;

extern CCNodeRGBA_setColor_fn CCNodeRGBA_setColor;
extern CCNodeRGBA_getColor_fn CCNodeRGBA_getColor;
extern CCNodeRGBA_getDisplayedColor_fn CCNodeRGBA_getDisplayedColor;
extern CCNodeRGBA_getOpacity_fn CCNodeRGBA_getOpacity;

// ------------------------------------------------------
constexpr auto COCOS_LIB_FILENAME = "libcocos2d.dll";
void loadCCFn();