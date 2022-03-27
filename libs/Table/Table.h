#pragma once

#include <unordered_map>

// the key is the column name and the value is the type
using Columns = std::unordered_map<std::string, std::string>;
using Data = std::vector<std::vector<std::string>>;

class Table
{
private:
    std::string name;
    Columns columns;
    Data data;

public:
    explicit Table(std::string name);

    // save the table to disk
    void Save() const;

    // insert a new row to table
    void Insert();
};


