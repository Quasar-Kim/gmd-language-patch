#include <Windows.h>

#include "CCClass.h"

using CCLabelBMFont_setString_fn = void(__thiscall*)(void* pThis, const char* newString, bool needUpdateLabel);
using CCString_initWithFormatAndValist_fn = bool(__thiscall*)(void* pThis, const char* format, va_list ap);
using CCLabelBMFont_setAlignment_fn = void(__thiscall*)(void* pThis, int alignment);
using CCNodeRGBA_getColor_fn = const void* (__thiscall*)(void* pThis);
using CCNode_getChildByTag_fn = void* (__thiscall*)(void* pThis, int tag);
using CCLabelBMFont_setAnchorPoint_fn = void(__thiscall*)(void* pThis, const CCPoint & anchorPoint);
using CCNode_getAnchorPoint_fn = void* (__thiscall*)(void* pThis);
using CCPoint_setPoint_fn = void(__thiscall*)(void* pThis, float x, float y);
using CCSprite_setPosition_fn = void(__thiscall*)(void* pThis, const CCPoint & position);
using CCNode_getPosition_fn = const CCPoint & (__thiscall*)(void* pThis);
using CCSprite_ignoreAnchorPointForPosition_fn = void(__thiscall*)(void* pThis, bool ignore);
using CCNode_getParent_fn = void* (__thiscall*)(void* pThis);
using CCNode_setPositionY_fn = void(__thiscall*)(void* pThis, float y);
using CCNode_getPositionXY_fn = void(__thiscall*)(void* pThis, float* x, float* y);
using CCNode_transform_fn = void* (__thiscall*)(void* pThis);
using CCDirector_getRunningScene_fn = void* (__thiscall*)(void* pThis);
using CCNode_addChild_fn = void(__thiscall*)(void* pThis, void* child, int zOrder, int tag);
using CCDirector_sharedDirector_fn = void* (*)();
using CCNode_removeFromParentAndCleanup_fn = void(__thiscall*)(void* pThis, bool cleanup);
using CCNode_setZOrder_fn = void(__thiscall*)(void* pThis, int zOrder);
using CCDirector_getVisibleSize_fn = void* (__thiscall*)(void* pThis);
using CCNode_getContentSize_fn = const CCSize & (__thiscall*)(void* pThis);
using CCNode_setPositionXY_fn = void(__thiscall*)(void* pThis, float x, float y);
using CCSprite_addChild_fn = void(__thiscall*)(void* pThis, void* child, int zOrder, int tag);
using CCNode_convertToNodeSpace_fn = CCPoint(__thiscall*)(void* pThis, void* worldPoint);
using CCLabelBMFont_limitLabelWidth_fn = void(__thiscall*)(void* pThis, float pa, float pb, float pc);
using CCSpriteBatchNode_addChild_fn = void(__thiscall*)(void* pThis, void* child);
using CCNode_convertToWorldSpace_fn = CCPoint(__thiscall*)(void* pThis, const CCPoint & nodePoint);