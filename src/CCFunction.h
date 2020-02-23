#include <Windows.h>

#include "CCType.h"

using CCLabelBMFont_setString_fn = void(__thiscall*)(void* pThis, const char* newString, bool needUpdateLabel);
using CCLabelBMFont_limitLabelWidth_fn = void(__thiscall*)(void* pThis, float pa, float pb, float pc);
using CCLabelBMFont_setAlignment_fn = void(__thiscall*)(void* pThis, int alignment);
using CCLabelBMFont_setAnchorPoint_fn = void(__thiscall*)(void* pThis, const CCPoint & anchorPoint);
using CCLabelBMFont_setColor_fn = void(__thiscall*)(void* pThis, const ccColor3B & color);
using CCLabelBMFont_create_fn = void*(*)(const char* str, const char* fntFile);

using CCSprite_setPosition_fn = void(__thiscall*)(void* pThis, const CCPoint & position);
using CCSprite_ignoreAnchorPointForPosition_fn = void(__thiscall*)(void* pThis, bool ignore);
using CCSprite_addChild_fn = void(__thiscall*)(void* pThis, void* child, int zOrder, int tag);
using CCSprite_setColor_fn = void(__thiscall*)(void* pThis, const ccColor3B & color);

using CCNode_getChildByTag_fn = void* (__thiscall*)(void* pThis, int tag);
using CCNode_getAnchorPoint_fn = void* (__thiscall*)(void* pThis);
// using CCNode_getPosition_fn = const CCPoint & (__thiscall*)(void* pThis);
using CCNode_getPosition_fn = void* (__thiscall*)(void* pThis);
using CCNode_getParent_fn = void* (__thiscall*)(void* pThis);
using CCNode_transform_fn = void* (__thiscall*)(void* pThis);
using CCNode_addChild_fn = void(__thiscall*)(void* pThis, void* child, int zOrder, int tag);
using CCNode_removeFromParentAndCleanup_fn = void(__thiscall*)(void* pThis, bool cleanup);
using CCNode_setZOrder_fn = void(__thiscall*)(void* pThis, int zOrder);
using CCNode_getContentSize_fn = const CCSize & (__thiscall*)(void* pThis);
using CCNode_setPositionXY_fn = void(__thiscall*)(void* pThis, float x, float y);
// using CCNode_convertToNodeSpace_fn = CCPoint(__thiscall*)(void* pThis, const CCPoint & worldPoint);
using CCNode_convertToNodeSpace_fn = CCPoint(__thiscall*)(void* pThis, void* worldPoint);
using CCNode_convertToWorldSpace_fn = CCPoint(__thiscall*)(void* pThis, const CCPoint & nodePoint);
using CCNode_setParent_fn = void(__thiscall*)(void* pThis, void* parent);

using CCDirector_getRunningScene_fn = void* (__thiscall*)(void* pThis);
using CCDirector_getVisibleSize_fn = void* (__thiscall*)(void* pThis);
using CCDirector_sharedDirector_fn = void* (*)();
using CCDirector_getWinSize_fn = CCSize (__thiscall*)(void* pThis);

using CCString_initWithFormatAndValist_fn = bool(__thiscall*)(void* pThis, const char* format, va_list ap);

using CCNodeRGBA_setColor_fn = void(__thiscall*)(void* pThis, const ccColor3B & color);
using CCNodeRGBA_getColor_fn = const ccColor3B & (__thiscall*)(void* pThis);

using CCSpriteBatchNode_addchild_fn = void(__thiscall*)(void* pThis, void* child, int zOrder, int tag);

using CCPoint_setPoint_fn = void(__thiscall*)(void* pThis, float x, float y);