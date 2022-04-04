#include "StringUtils.h"
#include <algorithm>

std::string StringUtils::s_WHITESPACE = " \n\t";

std::string StringUtils::LeftTrim(const std::string& str)
{
    std::string::size_type start = str.find_first_not_of(s_WHITESPACE);
    return (start == std::string::npos) ? "" : str.substr(start);
}

std::string StringUtils::RightTrim(const std::string& str)
{
    std::string::size_type end = str.find_last_not_of(s_WHITESPACE);
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

std::string StringUtils::Trim(const std::string& str)
{
    return LeftTrim(RightTrim(str));
}

void StringUtils::Replace(std::string& str, char val, char by)
{
    std::replace(str.begin(), str.end(), val, by);
}
