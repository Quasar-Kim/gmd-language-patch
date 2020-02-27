#pragma once
#include <string>
#include "CCStruct.h"

struct ColorInfo
{
	unsigned int startPos;
	unsigned int endPos;
	CCColor3B color;
};

std::vector<ColorInfo> parseColorMarkup(std::string str);
std::u16string utf8To16(std::string str);
std::string utf16To8(std::u16string str);
std::string removeColorMarkup(std::string str);