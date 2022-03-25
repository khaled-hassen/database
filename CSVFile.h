#pragma once

#include <fstream>
#include <string>
#include <vector>

using Row = std::vector<std::string>;
using Data = std::vector<Row>;

class CSVFile
{
private:
    std::fstream file;
    // the first row will contain the column names
    Data data;
public:
    // the default constructor
    CSVFile() = default;

    // create a new csv file
    void createNewFile(const std::string& filename);

    // save changes to disk
    void save();

    // close the file
    void close();

    // delete the file from disk
    void deleteFile(const std::string& filename);

    // create a new columnNames in csv file
    void createColumns(const Row& columnNames);

    // add a new data row
    void addRow(const Row& row);

    void read(const std::string& filename);
};


