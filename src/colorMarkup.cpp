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

// TODO: ���� �� �ʱ�ȭ �Լ�
void colorLabelStr(void* label, std::string utf8Str)
{
	// �ѱ� ���� index�� �߸��ǰ� ������ ������ �ذ��ϱ� ���� 
	// ���� UTF-8 ��� str�� UTF-16(wide char)�� �ٲߴϴ�
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
		// str�� </cr>�� ������ ��� endTagPos�� str�� ������ ������ �ε��� + 1 �̱� ������ �״�� ��� �� ������ ����ų �� �ֽ��ϴ�
		startTagPos = str.find(u"<c", endTagPos - 1);
	}
}