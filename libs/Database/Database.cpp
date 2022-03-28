#include "Database.h"
#include "../Table/Table.h"
#include "../CSVFile/CSVFile.h"
#include <filesystem>
#include <iostream>
#include <algorithm>

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
        if (file.extension() == ".csv")
        {
            std::string path = file.string();
            // Windows uses \ in path instead of /
            std::replace(path.begin(), path.end(), '\\', '/');
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
        unsigned start = entry.find_last_of("/");
        start = start == std::string::npos ? 0 : start + 1;
        std::cout << "* " << entry.substr(start, entry.size() - start - 4) << "\n";
    }

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

void Database::SaveTable()
{
    CheckOpenedDb();
    table->Save();
}

void Database::InsertRecord()
{
    CheckOpenedTable();
    table->Insert();
}

void Database::DropTable(const std::string& tableName)
{
    // delete the opened table
    if (tableName.empty())
    {
        CheckOpenedTable();
        std::string name = table->GetName();
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

void Database::ShowTable() const
{
    CheckOpenedTable();
    table->Show();
}

void Database::DeleteRecord(unsigned id)
{
    CheckOpenedTable();
    table->DeleteRecord(id);
}
