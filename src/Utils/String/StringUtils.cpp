#include "StringUtils.h"
#include <algorithm>

namespace StringUtils
{
    std::string LeftTrim(const std::string& str)
    {
        std::string::size_type start = str.find_first_not_of(" \n\t");
        return (start == std::string::npos) ? "" : str.substr(start);
    }

    std::string RightTrim(const std::string& str)
    {
        std::string::size_type end = str.find_last_not_of(" \n\t");
        return (end == std::string::npos) ? "" : str.substr(0, end + 1);
    }

    std::string Trim(const std::string& str) { return LeftTrim(RightTrim(str)); }

    void Replace(std::string& str, char val, char by) { std::replace(str.begin(), str.end(), val, by); }
}