#pragma once

#include "Utils/types/types.h"
#include <string>

class Table
{
private:
    std::string m_Path;
    std::string m_Name;
    Columns m_Columns;
    Data m_Data;

public:
    Table(std::string path, std::string name, Columns columns);

    Table(std::string path, std::string name, Columns columns, const Data& data);

    [[nodiscard]] inline const std::string& GetPath() const { return m_Path; }

    [[nodiscard]] inline const std::string& GetName() const { return m_Name; }

    [[nodiscard]] inline const Data& GetData() const { return m_Data; }

    [[nodiscard]] inline const Columns& GetColumns() const { return m_Columns; }

    // save the table to disk
    void Save() const;

    // insert a new row to table
    void InsertRecord();

    // display the data in the table
    void ShowRecords() const;

    // delete a record by id from table
    void DeleteRecord(unsigned id);

    // search table records and display results
    void SearchRecord() const;

    // update record by id
    void UpdateRecord(unsigned id);
};


