#include "Table.h"
#include "../Utils/ID/Id.h"
#include "../CSVFile/CSVFile.h"
#include "../Utils/String/StringUtils.h"
#include <iostream>
#include <utility>
#include <algorithm>

Table::Table(std::string name) : name(std::move(name))
{
    std::cout << "Creating table. Available data types: string, int, double \n";

    int number;
    std::string input;
    columns.clear();
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
    columns.emplace_back("id:ID");
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
        columns.push_back(columnName);
    }
    std::cout << std::endl;
}

Table::Table(std::string name, Columns columns, const Data& data) : name(std::move(name)), columns(std::move(columns)),
                                                                    data(data)
{
    std::string lastId = data.back().at("id");
    ID::SetLastID(std::stoi(lastId));
}

const std::string& Table::GetName() const { return name; }

void Table::Save() const
{
    CSVFile file(name);
    file.CreateNewFile();
    file.Save(columns, data);
}

void Table::Insert()
{
    auto it = columns.begin();
    ++it;
    unsigned size = columns.size();
    Record record;
    record.reserve(size + 1); // +1 for the id column
    record.insert({ "id", ID().ToString() });
    while (it != columns.end())
    {
        while (true)
        {
            unsigned index = it->find(":");
            std::string col = it->substr(0, index);
            std::string type = it->substr(index + 1);
            std::cout << col << "(" << type << "): ";
            std::string value;
            std::getline(std::cin, value);
            value = StringUtils::Trim(value);
            try
            {
                if (value.empty()) throw std::exception();
                if (type == "int") std::stoi(value); // throws exception if value not int
                else if (type == "double") std::stod(value); // throws exception if value not double
                // else the value type is string
                record.insert({ col, value });
                break;
            } catch (...)
            { std::cout << "Invalid value\n"; }
        }
        ++it;
    }
    data.push_back(record);
}

void Table::Show() const
{
    for (const auto& record: data)
    {
        for (const auto& val: record) std::cout << val.first << ":" << val.second << "\n";
        std::cout << std::endl;
    }
}

void Table::DeleteRecord(unsigned id)
{
    // std::find_if(data.begin(), data.end(), [](const Record& record) { return record.at(0); })
}
