#include "Database.h"
#include "../CSVFile/CSVFile.h"
#include "../Utils/String/StringUtils.h"
#include <filesystem>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;
std::string Database::BASE_NAME = "databases/";

std::vector<std::string> Database::ScanForDatabases()
{
    if (!fs::exists(BASE_NAME)) return std::vector<std::string>(0);

    std::vector<std::string> dbNames;
    for (const auto& entry: fs::directory_iterator(BASE_NAME))
        dbNames.push_back(entry.path().string().substr(BASE_NAME.length()));

    return dbNames;
}

void Database::CreateDb(const std::string& _dbName)
{
    CloseDb();
    if (!fs::exists(BASE_NAME))
    {
        bool success = fs::create_directory(BASE_NAME);
        if (!success) throw std::exception("Can't create database");
    }

    if (fs::exists(BASE_NAME + _dbName)) throw std::exception("Database exists");

    dbPath = BASE_NAME + _dbName;
    dbName = _dbName;
    bool success = fs::create_directory(dbPath);
    if (!success) throw std::exception("Can't create database");
    LoadTablesNames();
}

void Database::OpenDb(const std::string& _dbName)
{
    CloseDb();
    CheckDbExists(_dbName);
    dbPath = BASE_NAME + _dbName;
    dbName = _dbName;
    LoadTablesNames();
}

void Database::LoadTablesNames()
{
    for (const auto& entry: fs::directory_iterator(dbPath))
    {
        const fs::path& file = entry.path();
        if (file.extension() == ".csv")
        {
            const std::string& filename = file.filename().string();
            tableNames.push_back(filename.substr(0, filename.length() - 4)); // 4 is the length of ".csv"
        }
    }
}

void Database::CheckOpenedDb() const
{
    if (dbName.empty()) throw std::exception("There is no opened database");
}

void Database::CheckDbExists(const std::string& _dbName)
{
    if (!fs::exists(BASE_NAME + _dbName)) throw std::exception("Database doesn't exist");
}

void Database::ShowTables() const
{
    CheckOpenedDb();
    for (const auto& entry: tableNames)
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
    tableNames.clear();
    dbName = "";
    dbPath = "";
    CloseTable();
}

const std::string& Database::GetDbName() const { return dbName; }

const std::vector<std::string>& Database::GetTableNames() const { return tableNames; }

void Database::DropDb(const std::string& _dbName)
{
    const std::string& tmpPath = BASE_NAME + _dbName;
    if (_dbName == dbName) CloseDb(); // close the opened database if it's getting deleted
    else CheckDbExists(_dbName);
    fs::remove_all(tmpPath);
}

std::string Database::CreateTablePath(const std::string& tableName) const { return dbPath + "/" + tableName + ".csv"; }

void Database::CreateTable(const std::string& tableName, const Columns& cols)
{
    CheckOpenedDb();
    CloseTable();
    const std::string& filename = CreateTablePath(tableName);
    if (fs::exists(filename)) throw std::exception("Table exists");

    table = Pointer(new Table(filename, cols));
    table->Save();
    tableNames.push_back(tableName);
}

void Database::OpenTable(const std::string& tableName)
{
    CheckOpenedDb();
    CloseTable();
    const std::string& filename = CreateTablePath(tableName);
    CSVFile file(filename);
    Columns columns;
    Data data;
    file.Read(columns, data);
    table = Pointer(new Table(filename, columns, data));
}

const Pointer<Table>& Database::GetTable() const
{
    CheckOpenedTable();
    return table;
}

void Database::CheckOpenedTable() const
{
    CheckOpenedDb();
    if (table == nullptr) throw std::exception("There's no opened table");
}

void Database::CloseTable() { table = Pointer<Table>::GetNull(); }

void Database::DropTable()
{
    std::string name = GetTable()->GetName();
    std::string path = GetTable()->GetPath();
    CloseTable();
    // remove the tableName from the tableNames vector
    tableNames.erase(std::remove(tableNames.begin(), tableNames.end(), name), tableNames.end());
    CSVFile file(path);
    file.Delete();
}
