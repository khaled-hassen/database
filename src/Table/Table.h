#pragma once

#include "../Utils/types/types.h"
#include <string>

class Table
{
private:
    std::string name;
    Columns columns;
    Data data;

public:
    explicit Table(std::string name);

    Table(std::string name, Columns columns, const Data& data);

    [[nodiscard]] const std::string& GetName() const;

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
};

