#include "glew/glew.h"
#pragma once
typedef struct _ccColor3B
{
    GLubyte r;
    GLubyte g;
    GLubyte b;
} ccColor3B;

//! helper macro that creates an ccColor3B type
static inline ccColor3B
ccc3(const GLubyte r, const GLubyte g, const GLubyte b)
{
    ccColor3B c = { r, g, b };
    return c;
}