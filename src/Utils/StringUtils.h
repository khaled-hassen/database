#pragma once

#include <string>

namespace StringUtils
{
    std::string LeftTrim(const std::string& str);

    std::string RightTrim(const std::string& str);

    std::string Trim(const std::string& str);

    void Replace(std::string& str, char val, char by);
};


