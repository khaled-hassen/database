#pragma once

#include <string>

namespace StringUtils
{
    // remove all trailing left spaces from a string
    std::string LeftTrim(const std::string& str);

    // remove all trailing right spaces from a string
    std::string RightTrim(const std::string& str);

    // remove all trailing spaces from a string
    std::string Trim(const std::string& str);

    // replace a char by another and return the new string
    std::string Replace(const std::string& str, char val, char by);
}


