#include "pch.h"
#include <Windows.h>
// stdlib
#include <stdexcept>
// header
#include "CCFunction.h"

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
