#pragma once

#include <string>
#include <vector>
#include "Utils/Pointer.h"
#include "Table.h"
#include "Savable.h"

class Database : public Savable
{
private:
    static std::string s_Root;
    static std::string s_Extension;
    static std::string s_TableExtension;
    std::string m_Name;
    std::vector<std::string> m_TableNames;
    Pointer<Table> m_Table;
    bool m_Open = false;

private:
    [[nodiscard]] static std::string CreateTablePath(const std::string& tableName);

    // check if there's an opened database
    void CheckOpenedDb() const;

    // check if the database exists
    void CheckDbExists();

    // check if there's an opened table
    void CheckOpenedTable() const;

    // read the database tables from disk
    void Read() override;

    // save the database to disk
    void Save() override;

public:
    // scan the exe folder for available databases
    static std::vector<std::string> ScanForDatabases();

    explicit Database(const std::string& name);

    // create and open the database
    void CreateDb();

    // OpenDb the database: load all the tables into memory
    void OpenDb();

    // close the opened database
    void CloseDb();

    [[nodiscard]] inline const std::string& GetName() const { return m_Name; }

    [[nodiscard]] inline const std::vector<std::string>& GetTableNames() const { return m_TableNames; }

    // delete database
    static void DropDb(const std::string& dbName);

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


