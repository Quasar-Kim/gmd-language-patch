#pragma once

#include <vector>
#include <string>

#include "CCStruct.h"

struct charColorInfo
{
	unsigned int start;
	unsigned int length;
	ccColor3B charColor;
};

std::vector<charColorInfo> parseColorMarkup(std::string translatedStr);