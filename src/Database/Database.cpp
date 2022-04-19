#include "Database.h"
#include "Utils/StringUtils.h"
#include <filesystem>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;
std::string Database::s_BaseName = "databases/";

std::vector<std::string> Database::ScanForDatabases()
{
    if (!fs::exists(s_BaseName)) return std::vector<std::string>(0);

    std::vector<std::string> dbNames;
    for (const auto& entry: fs::directory_iterator(s_BaseName))
        dbNames.push_back(entry.path().string().substr(s_BaseName.length()));

    return dbNames;
}

void Database::CreateDb(const std::string& dbName)
{
    CloseDb();
    if (!fs::exists(s_BaseName))
    {
        bool success = fs::create_directory(s_BaseName);
        if (!success) throw std::exception("Can't create database");
    }

    if (fs::exists(s_BaseName + dbName)) throw std::exception("Database exists");

    m_Path = s_BaseName + dbName;
    m_Name = dbName;
    bool success = fs::create_directory(m_Path);
    if (!success) throw std::exception("Can't create database");
    LoadTablesNames();
}

void Database::OpenDb(const std::string& dbName)
{
    CloseDb();
    CheckDbExists(dbName);
    m_Path = s_BaseName + dbName;
    m_Name = dbName;
    LoadTablesNames();
}

void Database::LoadTablesNames()
{
    for (const auto& entry: fs::directory_iterator(m_Path))
    {
        const fs::path& file = entry.path();
        if (file.extension() == ".csv")
        {
            const std::string& filename = file.filename().string();
            m_TableNames.push_back(filename.substr(0, filename.length() - 4)); // 4 is the length of ".csv"
        }
    }
}

void Database::CheckOpenedDb() const
{
    if (!IsDbOpen()) throw std::exception("There is no opened database");
}

void Database::CheckDbExists(const std::string& dbName)
{
    if (!fs::exists(s_BaseName + dbName)) throw std::exception("Database doesn't exist");
}

void Database::ShowTables() const
{
    CheckOpenedDb();
    for (const auto& entry: m_TableNames)
    {
        // prevent the extension and the full path from being printed
        // 4 is the size of ".csv"
        std::string::size_type start = entry.find_last_of('/');
        start = start == std::string::npos ? 0 : start + 1;
        std::cout << "* " << entry.substr(start, entry.size() - start - 4) << "\n";
    }

    std::cout << std::endl;
}

void Database::CloseDb()
{
    m_TableNames.clear();
    m_Name = "";
    m_Path = "";
    CloseTable();
}

void Database::DropDb(const std::string& dbName)
{
    const std::string& tmpPath = s_BaseName + dbName;
    if (dbName == m_Name) CloseDb(); // close the opened database if it's getting deleted
    else CheckDbExists(dbName);
    fs::remove_all(tmpPath);
}

std::string Database::CreateTablePath(const std::string& tableName) const { return m_Path + "/" + tableName + ".tb"; }

void Database::CreateTable(const std::string& tableName, const Columns& cols)
{
    CheckOpenedDb();
    CloseTable();
    const std::string& path = CreateTablePath(tableName);
    if (fs::exists(path)) throw std::exception("Table exists");

    m_Table = Pointer(new Table(path, tableName, cols));
    m_Table->Save();
    m_TableNames.push_back(tableName);
}

void Database::OpenTable(const std::string& tableName)
{
    CheckOpenedDb();
    CloseTable();
    const std::string& path = CreateTablePath(tableName);
    m_Table = Pointer(new Table(path, tableName));
    m_Table->Read();
}

const Pointer<Table>& Database::GetTable() const
{
    CheckOpenedTable();
    return m_Table;
}

void Database::CheckOpenedTable() const
{
    CheckOpenedDb();
    if (m_Table == nullptr) throw std::exception("There's no opened table");
}

void Database::CloseTable() { m_Table = Pointer<Table>::GetNull(); }

void Database::DropTable()
{
    std::string name = GetTable()->GetName();
    std::string path = GetTable()->GetPath();
    m_Table->Delete();
    CloseTable();
    // remove the tableName from the tableNames vector
    m_TableNames.erase(std::remove(m_TableNames.begin(), m_TableNames.end(), name), m_TableNames.end());
}
