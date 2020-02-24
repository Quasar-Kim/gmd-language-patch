#pragma once
#include <cstdint>

// in Win32, GLubyte is defined as uint8_t
struct ccColor3B
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

static inline ccColor3B
ccc3(const uint8_t r, const uint8_t g, const uint8_t b)
{
    ccColor3B c = { r, g, b };
    return c;
}
