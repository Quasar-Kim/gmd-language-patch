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
/* NOTE: �� �ڵ�
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

	// ���� ����(\n)�� setOpacityModifyRGB�� ȣ������ �ʽ��ϴ�
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
		// str�� </cr>�� ������ ��� endTagPos�� str�� ������ ������ �ε��� + 1 �̱� ������ �״�� ��� �� ������ ����ų �� �ֽ��ϴ�
		startTagPos = str.find(u"<c", endTagPos - 1);
	}
	return result;
}

// �ѱ� ���� index�� �߸��ǰ� ������ ������ �ذ��ϱ� ���� 
// ���� UTF-8 ��� str�� UTF-16(wide char)�� �ٲߴϴ�
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