#include "pch.h"
// stdlib
#include <vector>
#include <string>
#include <stdexcept>
#include <codecvt>
#include <algorithm>
#include <regex>
// header
#include "CCClass.h"
#include "CCConst.h"
#include "CCFunction.h"
#include "colorMarkup.h"

// TODO: namespace
/* NOTE: 색 코드
	- g: Green
	- y: Gold(or Yellow)
	- l: Blue(or lime blue?)
	- p: Purple
	- r: Red
	- o: Orange
	- 
*/
std::vector<ColorInfo> parseColorMarkup(std::string utf8Str)
{
	std::vector<ColorInfo> result;

	// 개행 문자(\n)는 setOpacityModifyRGB를 호출하지 않습니다
	std::u16string str = utf8To16(utf8Str);
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());

	unsigned int startTagPos = str.find(u"<c");
	while (startTagPos != std::string::npos)
	{
		unsigned int endTagPos = str.find(u"</c>", startTagPos);
		unsigned int colorTypeDefPos = startTagPos + 2;
		if (endTagPos != std::string::npos)
		{
			CCColor3B color = colorWhite;
			switch (str[colorTypeDefPos])
			{
			case 'r':
				color = labelRed;
				break;
			case 'g':
				color = labelGreen;
				break;
			case 'y':
				color = labelYellow;
				break;
			case 'l':
				color = labelBlue;
				break;
			case 'p':
				color = labelPurple;
				break;
			case 'o':
				color = labelOrange;
				break;
			default:
				throw std::invalid_argument("Failed to parse color markup string: invalid color");
			}

			endTagPos -= 4;
			str.erase(startTagPos, 4);
			str.erase(endTagPos, 4);

			ColorInfo colorInfo = { startTagPos, endTagPos - 1, color };
			result.push_back(colorInfo);
		}
		else
		{
			throw std::invalid_argument("Failed to parse color markup string: cannot find matching end tag");
		}
		// str이 </cr>로 끝나는 경우 endTagPos는 str의 마지막 문자의 인덱스 + 1 이기 때문에 그대로 사용 시 오류를 일으킬 수 있습니다
		startTagPos = str.find(u"<c", endTagPos - 1);
	}
	return result;
}

// 한글 사용시 index가 잘못되게 나오는 문제를 해결하기 위해 
// 먼저 UTF-8 기반 str을 UTF-16(wide char)로 바꿉니다
std::u16string utf8To16(std::string str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16Conv;
	return utf16Conv.from_bytes(str);
}

std::string utf16To8(std::u16string str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16Conv;
	return utf16Conv.to_bytes(str);
}

std::string removeColorMarkup(std::string str)
{
	std::regex markup("(<c.?>)|(</c>)");
	return std::regex_replace(str, markup, "");
}