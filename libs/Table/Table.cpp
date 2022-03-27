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

    columns.reserve(number + 1);
    columns.insert({ "id", "ID" });
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
        columns.insert({ columnName, type });
    }
    std::cout << std::endl;
}

void Table::Save() const
{
    CSVFile file(name);
    Data tmpData;
    tmpData.reserve(data.size() + 1);
    std::vector<std::string> row;
    for (const auto& it: columns) row.push_back(it.first + ":" + it.second);
    tmpData.push_back(row);
    tmpData.insert(tmpData.end(), data.begin(), data.end());
    file.CreateNewFile();
    file.Save(tmpData);
    file.Close();
}

void Table::Insert()
{
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
            std::cout << it->first << "(" << it->second << "): ";
            std::string value;
            std::getline(std::cin, value);
            value = StringUtils::Trim(value);
            try
            {
                if (value.empty()) throw std::exception();
                if (it->second == "int") std::stoi(value); // throws exception if value not int
                else if (it->second == "double") std::stod(value); // throws exception if value not double
                // else the value is string
                row.push_back(value);
                break;
            } catch (...)
            { std::cout << "Invalid value\n"; }
        }
        ++it;
    }
    data.push_back(row);
}
