#include "pch.h"

#include <vector>
#include <string>
#include <stdexcept>

#include "CCClass.h"
#include "colorMarkup.h"

// y l 
std::vector<charColorInfo> parseColorMarkup(std::string translatedStr)
{
    std::vector<charColorInfo> strColorInfo;
    unsigned int colorDefStartPos = translatedStr.find("<c");
    while (colorDefStartPos != std::string::npos)
    {
        unsigned int colorTypePos = colorDefStartPos + 2;
        unsigned int colorStartPos = colorDefStartPos + 4;
        unsigned int colorDefEndPos = translatedStr.find("</c>", colorDefStartPos);
        if (colorStartPos < translatedStr.length())
        {
            charColorInfo colorInfo = { colorStartPos, 0, colorWhite };
            switch (translatedStr[colorTypePos])
            {
            case 'r':
                colorInfo.charColor = labelRed;
                break;
            case 'g':
                colorInfo.charColor = labelGreen;
                break;
            default:
                throw std::invalid_argument("Failed to parse color markup string: color not present inside tag");
            }

            if (colorDefEndPos != std::string::npos)
            {
                colorInfo.length = colorDefEndPos - colorStartPos;
            }
            else
            {
                throw std::invalid_argument("Failed to parse color markup string: cannot find matching end tag");
            }
            strColorInfo.push_back(colorInfo);
            colorDefStartPos = translatedStr.find("<c", colorDefEndPos + 3);
        }
        else
        {
            colorDefStartPos = std::string::npos;
        }
    }
    return strColorInfo;
}