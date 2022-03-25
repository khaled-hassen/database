#pragma once

#include <fstream>
#include <string>
#include <vector>

using Row = std::vector<std::string>;
using Data = std::vector<Row>;

class CSVFile
{
private:
    std::string filename;
    std::fstream file;
    // the first row will contain the column names
    Data data;
public:
    explicit CSVFile(std::string filename);

    // create a new csv file
    void createNewFile();

    // save changes to disk
    void save();

    // close the file
    void close();

    // delete the file from disk
    void deleteFile();

    // create a new columnNames in csv file
    void createColumns(const Row& columnNames);

    // add a new data row
    void addRow(const Row& row);

    // read data from the csv file
    void read();
};


