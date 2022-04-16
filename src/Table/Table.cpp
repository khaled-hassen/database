#include "Table.h"
#include "Utils/Id.h"
#include "CSVFile/CSVFile.h"
#include "Utils/StringUtils.h"
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
