#pragma once
#include <stdint.h>

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