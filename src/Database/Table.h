#pragma once

#include "Utils/types.h"
#include "Savable.h"
#include "Utils/IDGenerator.h"

class Table : public Savable
{
private:
    IDGenerator m_IDGenerator;
    long m_Index = -1;
    std::string m_Name;
    Columns m_Columns;
    Data m_Data;
    bool m_UnsavedChanges = false;

public:
    // to be called when creating a new table
    // pass the columns when creating a new table, don't pass them when opening a table
    Table(long index, std::string path, std::string name, Columns columns = {});

    // to be called when opening a saved table
    // Table(long index, std::string path, std::string name);

    [[nodiscard]] inline const std::string& GetName() const { return m_Name; }

    [[nodiscard]] inline const Data& GetData() const { return m_Data; }

    [[nodiscard]] inline const Record& GetRecord(long index) const { return m_Data.at(index); }

    [[nodiscard]] inline const Columns& GetColumns() const { return m_Columns; }

    [[nodiscard]] inline bool HasUnsavedChanged() const { return m_UnsavedChanges; }

    [[nodiscard]] inline long GetIndex() const { return m_Index; }

    // Read the table data stored in the file
    void Load() override;

    // save the table to file
    void Save() override;

    // insert a new row to table
    void InsertRecord(const Record& record);

    // delete a record from table
    void DeleteRecord(long index);

    // update record
    void UpdateRecord(long index, const Record& data);
};


