#include "StringUtils.h"
#include <algorithm>

namespace StringUtils
{
    std::string LeftTrim(const std::string& str)
    {
        size_t start = str.find_first_not_of(" \n\t");
        return (start == std::string::npos) ? "" : str.substr(start);
    }

    std::string RightTrim(const std::string& str)
    {
        size_t end = str.find_last_not_of(" \n\t");
        return (end == std::string::npos) ? "" : str.substr(0, end + 1);
    }

    std::string Trim(const std::string& str) { return LeftTrim(RightTrim(str)); }

    std::string Replace(const std::string& str, char val, char by)
    {
        std::string result = str;
        std::replace(result.begin(), result.end(), val, by);
        return result;
    }
}