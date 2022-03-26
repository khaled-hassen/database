#include "Database.h"
#include "../Utils/Pointer/Pointer.h"
#include "../Table/Table.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
std::string Database::BASE_NAME = "databases/";

void Database::Create(const std::string& _dbName)
{
    if (!fs::exists(BASE_NAME))
    {
        bool success = fs::create_directory(BASE_NAME);
        if (!success) throw std::exception("Can't Create database");
    }

    dbName = BASE_NAME + _dbName;
    if (fs::exists(dbName)) throw std::exception("Database exists");
    bool success = fs::create_directory(dbName);
    if (!success) throw std::exception("Can't Create database");
    LoadTablesNames();
}

void Database::Open(const std::string& _dbName)
{
    if (!fs::exists(BASE_NAME + _dbName)) throw std::exception("Database doesn't exist");
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

void Database::Show() const
{
    CheckOpenedDb();
    // Drop the extension from being printed
    // 4 is the size of ".csv"
    for (const auto& entry: tableNames)
        std::cout << "* " << entry.substr(0, entry.size() - 4) << "\n";

    std::cout << std::endl;
}

void Database::Close()
{
    CheckOpenedDb();
    tableNames.clear();
    dbName = "";
}

void Database::Drop(const std::string& _dbName)
{
    if (_dbName.empty())
    {
        // delete the opened db
        std::string tmp = dbName;
        Close();
        fs::remove_all(tmp);
    }
    else
    {
        // delete the provided _dbName
        if (!fs::exists(BASE_NAME + _dbName)) throw std::exception("Database doesn't exist");
        fs::remove_all(BASE_NAME + _dbName);
    }
}
