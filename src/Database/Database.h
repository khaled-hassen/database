#pragma once

#include <string>
#include <vector>
#include "Utils/Pointer/Pointer.h"
#include "Table/Table.h"

class Database
{
private:
    static std::string s_BASE_NAME;
    std::string m_Name;
    std::string m_Path;
    std::vector<std::string> m_TableNames;
    Pointer<Table> m_Table;

private:
    // load the database table names into memory
    void LoadTablesNames();

    // check if there's an opened database
    void CheckOpenedDb() const;

    // check if the database exists
    static void CheckDbExists(const std::string& dbName);

    // check if there's an opened table
    void CheckOpenedTable() const;

    [[nodiscard]] std::string CreateTablePath(const std::string& tableName) const;

public:
    // scan the exe folder for available databases
    static std::vector<std::string> ScanForDatabases();

    Database() = default;

    // create and open the database
    void CreateDb(const std::string& dbName);

    // OpenDb the database: load all the tables into memory
    void OpenDb(const std::string& dbName);

    // close the opened database
    void CloseDb();

    [[nodiscard]] inline const std::string& GetDbName() const { return m_Name; }

    [[nodiscard]] inline const std::vector<std::string>& GetTableNames() const { return m_TableNames; }

    // display the database tables
    void ShowTables() const;

    // delete database
    void DropDb(const std::string& dbName);

    // create a new database table
    void CreateTable(const std::string& tableName, const Columns& cols);

    // open a table
    void OpenTable(const std::string& tableName);

    // get the opened table. Throw an error when no table is opened
    [[nodiscard]] inline const Pointer<Table>& GetTable() const;

    // close the opened table
    void CloseTable();

    // delete the opened table
    void DropTable();
};


