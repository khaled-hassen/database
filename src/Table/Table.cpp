#include "Table.h"
#include "../Utils/ID/Id.h"
#include "../CSVFile/CSVFile.h"
#include "../Utils/String/StringUtils.h"
#include <iostream>
#include <utility>
#include <algorithm>

Table::Table(std::string path, Columns columns) : path(std::move(path)), columns(std::move(columns))
{
    unsigned start = this->path.find_last_of('/') + 1;
    unsigned end = this->path.find(".csv") - start;
    name = this->path.substr(start, end);
}

Table::Table(std::string path, Columns columns, const Data& data) : path(std::move(path)), columns(std::move(columns)),
                                                                    data(data)
{
    unsigned start = this->path.find_last_of('/') + 1;
    unsigned end = this->path.find(".csv") - start;
    name = this->path.substr(start, end);

    if (data.empty()) return;
    std::string lastId = data.back().at("id");
    ID::SetLastID(std::stoi(lastId));
}

const std::string& Table::GetName() const { return name; }

const std::string& Table::GetPath() const { return path; }

void Table::Save() const
{
    CSVFile file(path);
    file.CreateNewFile();
    file.Save(columns, data);
}

void Table::InsertRecord()
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

void Table::ShowRecords() const
{
    for (const auto& record: data)
    {
        for (const auto& val: record) std::cout << val.first << ":" << val.second << "\n";
        std::cout << std::endl;
    }
}

void Table::DeleteRecord(unsigned id)
{
    auto it = std::find_if(data.begin(), data.end(),
                           [id](const Record& record) { return record.at("id") == std::to_string(id); });
    if (it == data.end()) throw std::exception("This record doesn't exit");
    data.erase(it);
}

void Table::SearchRecord() const
{
    std::string colNames = "( ";
    for (const auto& col: columns) colNames += col + ", ";
    colNames = colNames.substr(0, colNames.size() - 2) + " )";

    std::string col;
    while (true)
    {
        std::cout << "Enter column to search by " << colNames << ": ";
        std::getline(std::cin, col);
        col = StringUtils::Trim(col);
        try
        {
            if (col.empty()) throw std::exception();
            // column doesn't exist
            if (colNames.find(col) == std::string::npos) throw std::exception();
            break;
        } catch (...)
        { std::cout << "Invalid column\n"; }

    }

    std::string value;
    while (true)
    {
        std::cout << "Enter value to search for: ";
        std::getline(std::cin, value);
        value = StringUtils::Trim(value);
        try
        {
            if (value.empty()) throw std::exception();
            break;
        } catch (...)
        { std::cout << "Invalid column\n"; }
    }

    bool found = false;
    for (const auto& record: data)
    {
        if (record.at(col) == value)
        {
            found = true;
            for (const auto& it: record) std::cout << it.first << ":" << it.second << "\n";
        }
    }
    if (!found) std::cout << "No record found" << std::endl;
}

void Table::UpdateRecord(unsigned int id)
{
    auto record = std::find_if(data.begin(), data.end(),
                               [id](const Record& record) { return record.at("id") == std::to_string(id); });
    if (record == data.end()) throw std::exception("This record doesn't exit");

    auto it = columns.begin();
    ++it;
    unsigned size = columns.size();
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
                record->at(col) = value;
                break;
            } catch (...)
            { std::cout << "Invalid value\n"; }
        }
        ++it;
    }
}