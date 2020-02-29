#pragma once
#include <string>
#include "json.hpp"

using json = nlohmann::json;

std::string translateFormat(std::string formattedStr, std::string formatStr, std::string defaultStr, const json& translation);