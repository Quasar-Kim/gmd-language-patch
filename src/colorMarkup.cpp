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
		// str�� </cr>�� ������ ��� endTagPos�� str�� ������ ������ �ε��� + 1 �̱� ������ ������ ����ų �� �ֽ��ϴ�
		startTagPos = str.find("<c", endTagPos - 1);
	}
}