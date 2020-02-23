#include "pch.h"

#include <vector>
#include <string>
#include <stdexcept>

#include "CCClass.h"
#include "CCConst.h"
#include "CCFunction.h"
#include "colorMarkup.h"

void colorLabelStr(void* label, std::string str)
{
	unsigned int startTagPos = str.find("<c");
	while (startTagPos != std::string::npos)
	{
		unsigned int endTagPos = str.find("</c>", startTagPos);
		unsigned int colorTypeDefPos = startTagPos + 2;
		if (endTagPos != std::string::npos)
		{
			// colorInfo.length = colorDefEndPos - coloringStartPos;
			// charColorInfo colorInfo = { coloringStartPos, 0, colorWhite };
			// ccColor3B color = ccWhite;
			ccColor3B color = colorWhite;
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
			default:
				throw std::invalid_argument("Failed to parse color markup string: invalid color");
			}

			// remove markup from string
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
		// str이 </cr>로 끝나는 경우 endTagPos는 str의 마지막 문자의 인덱스 + 1 이기 때문에 오류를 일으킬 수 있습니다
		startTagPos = str.find("<c", endTagPos - 1);
	}
}