#pragma once

#include <unordered_map>
#include <vector>
#include <string>

using Record = std::unordered_map<std::string, std::string>;
using Data = std::vector<Record>;
using Columns = std::unordered_map<std::string, std::string>;
