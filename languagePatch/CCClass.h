#pragma once

// cocos2dx�� ���Ǵ� Ŭ�������Դϴ�
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