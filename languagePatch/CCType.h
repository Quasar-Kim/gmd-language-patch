#pragma once

#include <stdint.h>

// cocos2dx에 사용되는 타입들입니다
// cocos2dx 함수 호출에 사용할 수 있습니다
//  Ex. CCSize myNodeSize = CCNode_getSize(myNode);
class CCSize
{
public:
    float width;
    float height;
};

class CCPoint
{
public:
    float x;
    float y;
public:
    CCPoint();
    CCPoint(float x, float y);
};

// in Win32, GLubyte is defined as uint8_t
typedef struct ccColor3B
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};