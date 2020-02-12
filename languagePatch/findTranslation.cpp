#include "pch.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
json translations;

bool loadTranslationfile(const char* fileName)
{
	std::ifstream translationFileStream(fileName);
	if (translationFileStream.is_open())
	{
		translationFileStream >> translations;
		return true;
	}
	return false;
}

// char findTranslation(const char* lookupStr, bool exact)
//{
	// 
//}