#pragma once
#include <cstdint>

// in Win32, GLubyte is defined as uint8_t
struct ccColor3B
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};