#include "Table.h"
#include "Utils/ID/Id.h"
#include "CSVFile/CSVFile.h"
#include "Utils/String/StringUtils.h"
#include <iostream>
#include <utility>
#include <algorithm>

Table::Table(std::string path, std::string name, Columns columns)
        : m_Path(std::move(path)), m_Name(std::move(name)), m_Columns(std::move(columns)) { }

Table::Table(std::string path, std::string name, Columns columns, const Data& data)
        : m_Path(std::move(path)), m_Name(std::move(name)), m_Columns(std::move(columns)), m_Data(data)
{
    if (data.empty()) return;
    std::string lastId = data.back().at("id");
    ID::SetLastID(std::stoi(lastId));
}

void Table::Save() const
{
    CSVFile file(m_Path);
    file.CreateNewFile();
    file.Save(m_Columns, m_Data);
}

void Table::InsertRecord()
{
    auto it = m_Columns.begin();
    ++it;
    unsigned size = m_Columns.size();
    Record record;
    record.reserve(size + 1); // +1 for the id column
    record.insert({ "id", ID().ToString() });
    while (it != m_Columns.end())
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
    m_Data.push_back(record);
}

void Table::ShowRecords() const
{
    for (const auto& record: m_Data)
    {
        for (const auto&[key, val]: record) std::cout << key << ":" << val << "\n";
        std::cout << std::endl;
    }
}

void Table::DeleteRecord(unsigned index)
{
    if (index >= m_Data.size() || index < 0) throw std::exception("This record doesn't exit");
    m_Data.erase(m_Data.cbegin() + index);
}

void Table::SearchRecord() const
{
    std::string colNames = "( ";
    for (const auto& col: m_Columns) colNames += col + ", ";
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
    for (const auto& record: m_Data)
    {
        if (record.at(col) == value)
        {
            found = true;
            for (const auto&[key, val]: record) std::cout << key << ":" << val << "\n";
        }
    }
    if (!found) std::cout << "No record found" << std::endl;
}

void Table::UpdateRecord(unsigned int id)
{
    auto record = std::find_if(m_Data.begin(), m_Data.end(),
                               [id](const Record& record) { return record.at("id") == std::to_string(id); });
    if (record == m_Data.end()) throw std::exception("This record doesn't exit");

    auto it = m_Columns.begin();
    ++it;
    unsigned size = m_Columns.size();
    while (it != m_Columns.end())
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
