#pragma once

#include <unordered_map>

// the key is the column name and the value is the type
using Columns = std::unordered_map<std::string, std::string>;

class Table
{
private:
    Columns columns;

public:
    Table();

    void Insert();
};


