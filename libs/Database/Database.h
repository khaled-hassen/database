#pragma once

#include <string>
#include <vector>

class Database
{
private:
    static std::string BASE_NAME;
    std::string dbName;
    std::vector<std::string> tableNames;

private:
    // load the database table names into memory
    void LoadTablesNames();

    // check if there's an opened database
    void CheckOpenedDb() const;

public:
    Database() = default;

    // Create and Open the database
    void Create(const std::string& _dbName);

    // Open the database: load all the tables into memory
    void Open(const std::string& _dbName);

    // Close the opened database
    void Close();

    // display the database tables
    void Show() const;

    // delete database. if _dbName name is provided delete _dbName database else delete the opened database
    void Drop(const std::string& _dbName = "");
};


