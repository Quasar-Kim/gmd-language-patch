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
CCLabelBMFont_setCascadeColorEnabled_fn CCLabelBMFont_setCascadeColorEnabled;

CCString_initWithFormatAndValist_fn CCString_initWithFormatAndValist;

CCNode_getChildByTag_fn CCNode_getChildByTag;
CCNode_addChild_fn CCNode_addChild;
CCNode_removeFromParentAndCleanup_fn CCNode_removeFromParentAndCleanup;
CCNode_getContentSize_fn CCNode_getContentSize;
CCNode_setParent_fn CCNode_setParent;
CCSprite_setVisible_fn CCSprite_setVisible;
CCNode_getChildren_fn CCNode_getChildren;
CCNode_getParent_fn CCNode_getParent;

CCObject_getTag_fn CCObject_getTag;

CCArray_randomObject_fn CCArray_randomObject;
CCArray_indexOf_fn CCArray_indexOf;

CCSprite_setPosition_fn CCSprite_setPosition;
CCSprite_addChild_fn CCSprite_addChild;
CCSprite_setColor_fn CCSprite_setColor;
CCSprite_setBatchNode_fn CCSprite_setBatchNode;
CCSprite_initWithTexture_fn CCSprite_initWithTexture;
CCSprite_setOpacityModifyRGB_fn CCSprite_setOpacityModifyRGB;

CCDirector_sharedDirector_fn CCDirector_sharedDirector;
CCDirector_getRunningScene_fn CCDirector_getRunningScene;
CCDirector_getVisibleSize_fn CCDirector_getVisibleSize;

CCNodeRGBA_setColor_fn CCNodeRGBA_setColor;
CCNodeRGBA_getColor_fn CCNodeRGBA_getColor;
CCNodeRGBA_getDisplayedColor_fn CCNodeRGBA_getDisplayedColor;
CCNodeRGBA_getOpacity_fn CCNodeRGBA_getOpacity;


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
		CCSprite_setVisible = reinterpret_cast<CCSprite_setVisible_fn>(GetProcAddress(cocosLib, "?setVisible@CCSprite@cocos2d@@UAEX_N@Z"));
		CCSprite_setBatchNode = reinterpret_cast<CCSprite_setBatchNode_fn>(GetProcAddress(cocosLib, "?setBatchNode@CCSprite@cocos2d@@UAEXPAVCCSpriteBatchNode@2@@Z"));
		CCNode_getChildren = reinterpret_cast<CCNode_getChildren_fn>(GetProcAddress(cocosLib, "?getChildren@CCNode@cocos2d@@UAEPAVCCArray@2@XZ"));
		CCArray_randomObject = reinterpret_cast<CCArray_randomObject_fn>(GetProcAddress(cocosLib, "?randomObject@CCArray@cocos2d@@QAEPAVCCObject@2@XZ"));
		CCNode_getParent = reinterpret_cast<CCNode_getParent_fn>(GetProcAddress(cocosLib, "?getParent@CCNode@cocos2d@@UAEPAV12@XZ"));
		CCSprite_initWithTexture = reinterpret_cast<CCSprite_initWithTexture_fn>(GetProcAddress(cocosLib, "?initWithTexture@CCSprite@cocos2d@@UAE_NPAVCCTexture2D@2@ABVCCRect@2@_N@Z"));
		CCSprite_setOpacityModifyRGB = reinterpret_cast<CCSprite_setOpacityModifyRGB_fn>(GetProcAddress(cocosLib, "?setOpacityModifyRGB@CCSprite@cocos2d@@UAEX_N@Z"));
		CCArray_indexOf = reinterpret_cast<CCArray_indexOf_fn>(GetProcAddress(cocosLib, "?indexOfObject@CCArray@cocos2d@@QBEIPAVCCObject@2@@Z"));
		CCObject_getTag = reinterpret_cast<CCObject_getTag_fn>(GetProcAddress(cocosLib, "?getTag@CCObject@cocos2d@@UBEHXZ"));
		CCNodeRGBA_getDisplayedColor = reinterpret_cast<CCNodeRGBA_getDisplayedColor_fn>(GetProcAddress(cocosLib, "?getDisplayedColor@CCLayerRGBA@cocos2d@@UAEABU_ccColor3B@2@XZ"));
		CCNodeRGBA_getOpacity = reinterpret_cast<CCNodeRGBA_getOpacity_fn>(GetProcAddress(cocosLib, "?getOpacity@CCNodeRGBA@cocos2d@@UAEEXZ"));
		CCLabelBMFont_setCascadeColorEnabled = reinterpret_cast<CCLabelBMFont_setCascadeColorEnabled_fn>(GetProcAddress(cocosLib, "?setCascadeColorEnabled@CCLabelBMFont@cocos2d@@UAEX_N@Z"));
	}
	else
	{
		throw std::runtime_error("Failed to load function from cocos2dlib.dll");
	}
}
