#pragma once

#include <string>

class StringUtils
{
private:
    static std::string s_WHITESPACE;

    static std::string LeftTrim(const std::string& str);

    static std::string RightTrim(const std::string& str);

public:
    // delete the constructor ( this class cannot be instantiated)
    StringUtils() = delete;

    static std::string Trim(const std::string& str);

    static void Replace(std::string& str, char val, char by);
};


