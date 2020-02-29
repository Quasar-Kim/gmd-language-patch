#include <string>
#include <regex>
#include "json.hpp"

using json = nlohmann::json;

std::string translateFormat(std::string formattedStr, std::string formatStr, std::string defaultStr, const json& translation)
{
	std::string translatedStr;
	if (formatStr.length() > 0)
	{
		auto translationEntry_it = translation.find(formatStr);
		if (translationEntry_it != translation.end())
		{
			// 1. regex 이용해 formatStr에서 placeholder 위치 찾아내기
			json translationEntry = *translationEntry_it;
			std::string translatedFormatStr = translationEntry["format"];
			std::vector<std::string> dynamicParts;
			std::vector<int> dynamicPartsPos;
			std::smatch matchResult;
			std::regex matchPlaceholder("%(?:c|s|u|f|F|d)");

			std::string::const_iterator searchStart(formatStr.cbegin());
			int skippedWhileSearch = 0;
			while (std::regex_search(searchStart, formatStr.cend(), matchResult, matchPlaceholder))
			{
				dynamicPartsPos.push_back(matchResult.position() + skippedWhileSearch);
				searchStart = matchResult.suffix().first;
				skippedWhileSearch += matchResult.position() + matchResult.length();
			}

			for (size_t i = 0; i < dynamicPartsPos.size(); ++i)
			{
				int placeholderPos = dynamicPartsPos[i];
				static int nextDynamicPartStart = placeholderPos;
				// 2. formatStr의 placeholder + 1부터 다음 placeholder까지의 substr 가져오기
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
				// 3. lookupStr에서 2의 결과 이용해서 dynamicPart를 찾아내기
				int nextStaticPartStart = formattedStr.find(nextStaticPart, nextDynamicPartStart);
				if (nextStaticPartStart != std::string::npos)
				{
					// 4. 나중에 번역하기 위해 vector에 dynamicPart 저장
					std::string dynamicPart = formattedStr.substr(nextDynamicPartStart, nextStaticPartStart - nextDynamicPartStart);
					dynamicParts.push_back(dynamicPart);
					nextDynamicPartStart += dynamicPart.length() + nextStaticPart.length();
				}
				else
				{
					throw std::invalid_argument("Error while transalting formatted text - the given formatStr cannot be formatted to the given formattedStr");
				}
			}

			// 5. translatedFormatStr을 regex 이용해 덮어쓰기
			// key - dest(translatedFormatStr)의 index
			for (const auto& item : translationEntry["parts"].items())
			{
				auto partEntry = item.value();
				
				int sourcePartIndex = partEntry["_sourcePart"];
				std::string sourcePart = dynamicParts[sourcePartIndex];
				std::string translatedPart = partEntry.value(sourcePart, sourcePart);

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