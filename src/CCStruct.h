#pragma once
#include <cstdint>

// in Win32, GLubyte is defined as uint8_t
struct CCColor3B
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

static inline CCColor3B
ccc3(const uint8_t r, const uint8_t g, const uint8_t b)
{
    CCColor3B c = { r, g, b };
    return c;
}
