#include "Database.h"
#include "../Table/Table.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
std::string Database::BASE_NAME = "databases/";

void Database::CreateDb(const std::string& _dbName)
{
    if (!fs::exists(BASE_NAME))
    {
        bool success = fs::create_directory(BASE_NAME);
        if (!success) throw std::exception("Can't create database");
    }

    dbName = BASE_NAME + _dbName;
    if (fs::exists(dbName)) throw std::exception("Database exists");
    bool success = fs::create_directory(dbName);
    if (!success) throw std::exception("Can't create database");
    LoadTablesNames();
}

void Database::OpenDb(const std::string& _dbName)
{
    CheckDbExists(_dbName);
    dbName = BASE_NAME + _dbName;
    LoadTablesNames();
}

void Database::LoadTablesNames()
{
    for (const auto& entry: fs::directory_iterator(dbName))
    {
        const fs::path& file = entry.path();
        if (file.extension() == ".csv") tableNames.push_back(file.filename().string());
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
    // prevent the extension from being printed
    // 4 is the size of ".csv"
    for (const auto& entry: tableNames)
        std::cout << "* " << entry.substr(0, entry.size() - 4) << "\n";

    std::cout << std::endl;
}

void Database::CloseDb()
{
    CheckOpenedDb();
    tableNames.clear();
    dbName = "";
    table = Pointer<Table>::GetNull();
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

    table = Pointer(new Table());
}

void Database::CheckOpenedTable() const
{
    if (table == nullptr) throw std::exception("There's no opened table");
}

void Database::CloseTable()
{
    CheckOpenedTable();
    table = Pointer<Table>::GetNull();
}

void Database::InsertRecord()
{
    CheckOpenedTable();
    table->Insert();
}
