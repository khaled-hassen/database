#pragma once

#include <unordered_map>

// the first row contains the columns names and types
using Data = std::vector<std::vector<std::string>>;

class Table
{
private:
    std::string name;
    Data data;

public:
    explicit Table(std::string name);

    Table(std::string name, const Data& _data);

    [[nodiscard]] const std::string& GetName() const;

    // save the table to disk
    void Save() const;

    // insert a new row to table
    void Insert();

};


