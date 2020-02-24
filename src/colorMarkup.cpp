#include "pch.h"
// stdlib
#include <vector>
#include <string>
#include <stdexcept>
#include <codecvt>
// header
#include "CCClass.h"
#include "CCConst.h"
#include "CCFunction.h"
#include "colorMarkup.h"

// TODO: 글자 색 초기화 함수
void colorLabelStr(void* label, std::string utf8Str)
{
	// 한글 사용시 index가 잘못되게 나오는 문제를 해결하기 위해 
	// 먼저 UTF-8 기반 str을 UTF-16(wide char)로 바꿉니다
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16Conv;
	std::u16string str = utf16Conv.from_bytes(utf8Str);

	unsigned int startTagPos = str.find(u"<c");
	while (startTagPos != std::string::npos)
	{
		unsigned int endTagPos = str.find(u"</c>", startTagPos);
		unsigned int colorTypeDefPos = startTagPos + 2;
		if (endTagPos != std::string::npos)
		{
			ccColor3B color = colorWhite;
			switch (str[colorTypeDefPos])
			{
			case 'r':
				// color = labelRed;
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
			default:
				throw std::invalid_argument("Failed to parse color markup string: invalid color");
			}

			endTagPos -= 4;
			str.erase(startTagPos, 4);
			str.erase(endTagPos, 4);

			for (unsigned int i = startTagPos; i < endTagPos; i++)
			{
				void* charSprite = CCNode_getChildByTag(label, i);
				CCSprite_setColor(charSprite, color);
			}
		}
		else
		{
			throw std::invalid_argument("Failed to parse color markup string: cannot find matching end tag");
		}
		// str이 </cr>로 끝나는 경우 endTagPos는 str의 마지막 문자의 인덱스 + 1 이기 때문에 그대로 사용 시 오류를 일으킬 수 있습니다
		startTagPos = str.find(u"<c", endTagPos - 1);
	}
}