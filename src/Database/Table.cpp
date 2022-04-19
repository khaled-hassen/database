#include "Table.h"
#include "Utils/Id.h"
#include <utility>
#include <algorithm>

Table::Table(std::string path, std::string name, Columns columns)
        : Savable(std::move(path)), m_Name(std::move(name)), m_Columns(std::move(columns)) { }

Table::Table(std::string path, std::string name)
        : Savable(std::move(path)), m_Name(std::move(name)) { }

void Table::InsertRecord(const Record& record)
{
    Record tmp;
    tmp.reserve(record.size() + 1);
    tmp.insert({ "id", ID().ToString() });
    tmp.insert(record.cbegin(), record.cend());
    m_Data.push_back(tmp);
}

void Table::DeleteRecord(long index)
{
    if (index >= m_Data.size() || index < 0) throw std::exception("This record doesn't exit");
    m_Data.erase(m_Data.cbegin() + index);
}

void Table::UpdateRecord(long index, const Record& data)
{
    Record& record = m_Data.at(index);

    for (const auto&[key, value]: data)
    {
        if (key == "id") continue;
        record.at(key) = value;
    }
}

void Table::Read()
{
    OpenFile();

    size_t colsSize = ReadSize();
    m_Columns.resize(colsSize);
    for (auto& col: m_Columns) col = ReadString();

    size_t dataSize = ReadSize();
    m_Data.resize(dataSize);
    for (auto& record: m_Data)
    {
        size_t recordSize = ReadSize();
        record.reserve(recordSize);
        for (size_t i = 0; i < recordSize; ++i)
        {
            std::string key = ReadString(), value = ReadString();
            record[key] = value;
        }
    }

    CloseFile();

    if (m_Data.empty()) return;
    std::string lastId = m_Data.back().at("id");
    ID::SetLastID(std::stoi(lastId));
}

void Table::Save()
{
    CreateFile();
    WriteSize(m_Columns.size());
    for (const auto& col: m_Columns) WriteString(col);

    WriteSize(m_Data.size());
    for (const auto& record: m_Data)
    {
        WriteSize(record.size());
        for (const auto&[key, value]: record)
        {
            WriteString(key);
            WriteString(value);
        }
    }

    CloseFile();
}

