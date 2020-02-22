#pragma once

#include <stdint.h>

// cocos2dx�� ���Ǵ� Ÿ�Ե��Դϴ�
// cocos2dx �Լ� ȣ�⿡ ����� �� �ֽ��ϴ�
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