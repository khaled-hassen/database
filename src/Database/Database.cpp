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

    std::vector<std::string> paths;
    for (const auto& entry: fs::directory_iterator(BASE_NAME))
        paths.push_back(entry.path().string().erase(0, BASE_NAME.length()));


    return paths;
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

    dbName = BASE_NAME + _dbName;
    bool success = fs::create_directory(dbName);
    if (!success) throw std::exception("Can't create database");
    LoadTablesNames();
}

void Database::OpenDb(const std::string& _dbName)
{
    CloseDb();
    CheckDbExists(_dbName);
    dbName = BASE_NAME + _dbName;
    LoadTablesNames();
}

void Database::LoadTablesNames()
{
    for (const auto& entry: fs::directory_iterator(dbName))
    {
        const fs::path& file = entry.path();
        if (file.extension() == ".csv")
        {
            std::string path = file.string();
            // Windows uses \ in path instead of /
            StringUtils::Replace(path, '\\', '/');
            tableNames.push_back(path);
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
    table = Pointer<Table>::GetNull();
}

std::vector<std::string> Database::GetTableNames() const
{
    CheckOpenedDb();
    std::vector<std::string> names;
    names.reserve(tableNames.size());
    for (const auto& name: tableNames)
    {
        unsigned start = name.find_last_of('/') + 1;
        unsigned end = name.find(".csv") - start;
        names.push_back(name.substr(start, end));
    }
    return names;
}

void Database::DropDb(const std::string& _dbName)
{
    if (_dbName.empty())
    {
        // delete the opened db
        std::string tmp = dbName;
        CloseDb();
        fs::remove_all(tmp);
    }
    else
    {
        // delete the provided _dbName
        CheckDbExists(_dbName);
        fs::remove_all(BASE_NAME + _dbName);
    }
}

void Database::CreateTable(const std::string& tableName)
{
    CheckOpenedDb();
    std::string filename = dbName + "/" + tableName + ".csv";
    if (fs::exists(filename)) throw std::exception("Table exists");

    table = Pointer(new Table(filename));
}

void Database::OpenTable(const std::string& tableName)
{
    CheckOpenedDb();
    if (table != nullptr) throw std::exception("There is an open table, close it before opening a new one");
    std::string filename = *GetDbTable(tableName);
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

std::vector<std::string>::iterator Database::GetDbTable(const std::string& tableName)
{
    std::string filename = dbName + "/" + tableName + ".csv";
    auto it = std::find(tableNames.begin(), tableNames.end(), filename);
    if (it == tableNames.end()) throw std::exception("This table isn't in the database");
    return it;
}

void Database::CloseTable()
{
    CheckOpenedTable();
    table = Pointer<Table>::GetNull();
}

void Database::DropTable(const std::string& tableName)
{
    // delete the opened table
    if (tableName.empty())
    {
        std::string name = GetTable()->GetName();
        table = Pointer<Table>::GetNull();
        // remove the tableName from the tableNames vector
        tableNames.erase(std::remove(tableNames.begin(), tableNames.end(), name), tableNames.end());
        CSVFile file(name);
        file.Delete();
        return;
    }

    CheckOpenedDb();
    // delete the passed tableName
    auto it = GetDbTable(tableName);
    std::string filename = *it;
    tableNames.erase(it);
    CSVFile file(filename);
    file.Delete();
}

