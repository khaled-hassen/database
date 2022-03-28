#include "Table.h"
#include "../Utils/ID/Id.h"
#include "../CSVFile/CSVFile.h"
#include "../Utils/String/StringUtils.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

Table::Table(std::string name) : name(std::move(name))
{
    std::cout << "Creating table. Available data types: string, int, double \n";

    int number;
    std::string input;
    data.clear();
    while (true)
    {
        std::cout << "Enter the number of columns: ";
        std::getline(std::cin, input);
        input = StringUtils::Trim(input);
        try
        {
            number = std::stoi(input);
        } catch (...)
        {
            std::cout << "Invalid number\n";
            continue;
        }
        if (number > 0) break;
        std::cout << "Invalid number\n";
    }

    std::vector<std::string> row;
    row.reserve(number + 1);
    row.emplace_back("id:ID");
    int i = 1;
    while (i <= number)
    {
        std::cout << "Column" << i << " name: ";
        std::string columnName;
        std::getline(std::cin, columnName);
        columnName = StringUtils::Trim(columnName);
        if (columnName.empty())
        {
            std::cout << "Enter a column name\n";
            continue;
        }

        std::string type;
        while (true)
        {
            std::cout << "Column" << i << " data type: ";
            std::getline(std::cin, type);
            type = StringUtils::Trim(type);
            if (type == "string" || type == "int" || type == "double")
            {
                ++i;
                break;
            }
            std::cout << "Invalid data type\n";
        }
        // replace all spaces with dashes
        std::replace(columnName.begin(), columnName.end(), ' ', '-');
        columnName += ":" + type;
        row.push_back(columnName);
    }
    data.push_back(row);
    std::cout << std::endl;
}

Table::Table(std::string name, const Data& _data) : name(std::move(name))
{
    if (_data.empty()) throw std::exception("The data must at least have the column names");
    data = _data;
    std::string lastId = _data.back().at(0);
    ID::SetLastID(std::stoi(lastId));
}

const std::string& Table::GetName() const { return name; }

void Table::Save() const
{
    CSVFile file(name);
    file.CreateNewFile();
    file.Save(data);
}

void Table::Insert()
{
    const auto& columns = data.at(0);
    auto it = columns.begin();
    ++it;
    unsigned size = columns.size();
    std::vector<std::string> row;
    row.reserve(size);
    row.push_back(ID().ToString());
    while (it != columns.end())
    {
        while (true)
        {
            unsigned index = it->find(":");
            std::string type = it->substr(index + 1);
            std::cout << it->substr(0, index) << "(" << type << "): ";
            std::string value;
            std::getline(std::cin, value);
            value = StringUtils::Trim(value);
            try
            {
                if (value.empty()) throw std::exception();
                if (type == "int") std::stoi(value); // throws exception if value not int
                else if (type == "double") std::stod(value); // throws exception if value not double
                // else the value type is string
                row.push_back(value);
                break;
            } catch (...)
            { std::cout << "Invalid value\n"; }
        }
        ++it;
    }
    data.push_back(row);
}
