#pragma once

#include <vector>
#include <string>

#include "CCType.h"

struct charColorInfo
{
	unsigned int start;
	unsigned int length;
	ccColor3B charColor;
};

const ccColor3B colorWhite = { 255, 255, 255 };
const ccColor3B labelRed = { 255, 0, 0 };
const ccColor3B labelYellow = { 255, 255, 0 };
const ccColor3B labelGreen = { 0, 255, 0 };
const ccColor3B labelBlue = { 0, 0, 255 };

std::vector<charColorInfo> parseColorMarkup(std::string translatedStr);