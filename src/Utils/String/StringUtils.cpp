#include "StringUtils.h"

std::string StringUtils::WHITESPACE = " \n\t";

std::string StringUtils::LeftTrim(const std::string& str)
{
    unsigned start = str.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : str.substr(start);
}

std::string StringUtils::RightTrim(const std::string& str)
{
    unsigned end = str.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

std::string StringUtils::Trim(const std::string& str)
{
    return LeftTrim(RightTrim(str));
}
