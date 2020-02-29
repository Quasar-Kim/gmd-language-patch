#include "pch.h"
#include <string>
#include <regex>
#include <algorithm>
#include "json.hpp"
#include "translateFormat.h"
#include "colorMarkup.h"

using json = nlohmann::json;

std::string translateFormat(std::string formattedStr, std::string rawFormatStr, std::string defaultStr, const json& translation)
{
	std::string translatedStr;
	if (rawFormatStr.length() > 0)
	{
		auto translationEntry_it = translation.find(rawFormatStr);
		if (translationEntry_it != translation.end())
		{
			// \n -> ����, �� ǥ�� ����
			std::string formatStr = removeColorMarkup(rawFormatStr);
			std::replace(formatStr.begin(), formatStr.end(), '\n', ' ');
			formatStr = std::regex_replace(formatStr, std::regex("%%"), "%");

			// 1. regex �̿��� formatStr���� placeholder ��ġ ã�Ƴ���
			json translationEntry = *translationEntry_it;
			std::string translatedFormatStr = translationEntry["format"];
			std::vector<std::string> dynamicParts;
			std::vector<int> dynamicPartsPos;
			std::smatch matchResult;
			std::regex matchPlaceholder("%(?:c|i|s|u|f|F|d)");

			std::string::const_iterator searchStart(formatStr.cbegin());
			int skippedWhileSearch = 0;

			while (std::regex_search(searchStart, formatStr.cend(), matchResult, matchPlaceholder))
			{
				dynamicPartsPos.push_back(matchResult.position() + skippedWhileSearch);
				searchStart = matchResult.suffix().first;
				skippedWhileSearch += matchResult.position() + matchResult.length();
			}

			int nextDynamicPartStart;
			for (size_t i = 0; i < dynamicPartsPos.size(); ++i)
			{
				int placeholderPos = dynamicPartsPos[i];
				if (i == 0)
				{
					nextDynamicPartStart = placeholderPos;
				}
				// 2. formatStr�� placeholder + 1���� ���� placeholder������ substr ��������
				std::string nextStaticPart;
				if (i + 1 == dynamicPartsPos.size())
				{
					nextStaticPart = formatStr.substr(placeholderPos + 2);
				}
				else
				{
					int nextPlaceholderPos = dynamicPartsPos[i + 1];
					nextStaticPart = formatStr.substr(placeholderPos + 2, nextPlaceholderPos - placeholderPos - 2);
				}
				// 3. lookupStr���� 2�� ��� �̿��ؼ� dynamicPart�� ã�Ƴ���
				int nextStaticPartStart = formattedStr.find(nextStaticPart, nextDynamicPartStart);
				if (nextStaticPartStart != std::string::npos)
				{
					// 4. ���߿� �����ϱ� ���� vector�� dynamicPart ����
					std::string dynamicPart = formattedStr.substr(nextDynamicPartStart, nextStaticPartStart - nextDynamicPartStart);
					dynamicParts.push_back(dynamicPart);
					nextDynamicPartStart += dynamicPart.length() + nextStaticPart.length();
				}
				else
				{
					throw std::invalid_argument("Error while transalting formatted text - the given formatStr cannot be formatted to the given formattedStr");
				}
			}

			std::vector<std::string> translatedDynamicParts = dynamicParts;
			auto partsEntry = translationEntry["parts"];
			for (size_t i = 0; i < dynamicParts.size(); ++i)
			{
				if (i < partsEntry.size())
				{
					auto sourceDynamicPart = dynamicParts[i];
					int destPartIndex = partsEntry[i].value("_destPart", i);
					std::string translatedPart = partsEntry[i].value(sourceDynamicPart, sourceDynamicPart);
					translatedDynamicParts[destPartIndex] = translatedPart;
				}
			}

			for (auto& translatedPart : translatedDynamicParts)
			{
				translatedFormatStr = std::regex_replace(translatedFormatStr, matchPlaceholder, translatedPart, std::regex_constants::format_first_only);
			}
			translatedStr = translatedFormatStr;
		}
		else
		{
			translatedStr = defaultStr;
		}
	}
	else
	{
		throw std::invalid_argument("Error while transalting formatted text - the given formatStr is empty");
	}
	return translatedStr;
}