#pragma once

#include <string>
#include <vector>
#include "../Utils/Pointer/Pointer.h"
#include "../Table/Table.h"

class Database
{
private:
    static std::string BASE_NAME;
    std::string dbName;
    std::vector<std::string> tableNames;
    Pointer<Table> table;

private:
    // load the database table names into memory
    void LoadTablesNames();

    // check if there's an opened database
    void CheckOpenedDb() const;

    // check if the database exists
    static void CheckDbExists(const std::string& _dbName);

    // check if there's an opened table
    void CheckOpenedTable() const;

    // return an iterator that points to the location of the tableName in tableNames. Throws an exception if tableName not in tableNames
    std::vector<std::string>::iterator GetDbTable(const std::string& tableName);

public:
    Database() = default;

    // create and open the database
    void CreateDb(const std::string& _dbName);

    // OpenDb the database: load all the tables into memory
    void OpenDb(const std::string& _dbName);

    // close the opened database
    void CloseDb();

    // display the database tables
    void ShowTables() const;

    // delete database. if _dbName name is provided delete _dbName database else delete the opened database
    void DropDb(const std::string& _dbName = "");

    // create a new database table
    void CreateTable(const std::string& tableName);

    // open a table
    void OpenTable(const std::string& tableName);

    // get the opened table. Throw an error when no table is opened
    [[nodiscard]] const Pointer<Table>& GetTable() const;

    // close the opened table
    void CloseTable();

    // delete table. if tableName is provided delete tableName else delete the opened table
    void DropTable(const std::string& tableName = "");
};


