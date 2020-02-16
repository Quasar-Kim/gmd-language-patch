#pragma once

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
typedef struct _ccColor3B
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} ccColor3B;

//! helper macro that creates an ccColor3B type
static inline ccColor3B
ccc3(const uint8_t r, const uint8_t g, const uint8_t b)
{
    ccColor3B c = { r, g, b };
    return c;
}