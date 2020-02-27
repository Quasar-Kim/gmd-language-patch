#pragma once
#include <string>
#include "CCStruct.h"

struct StrColorInfo
{
	unsigned int startPos;
	unsigned int endPos;
	ccColor3B color;
};

std::vector<StrColorInfo> parseColorMarkup(std::string str);
std::u16string utf8To16(std::string str);
std::string utf16To8(std::u16string str);
std::string removeColorMarkup(std::string str);