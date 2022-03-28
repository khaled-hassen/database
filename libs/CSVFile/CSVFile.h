#pragma once

#include <fstream>
#include <string>
#include <vector>

using Data = std::vector<std::vector<std::string>>;

class CSVFile
{
private:
    std::string filename;
    std::fstream file;

private:
    // close the file
    void Close();

public:
    explicit CSVFile(std::string filename);

    virtual ~CSVFile();

    // create a new csv file
    void CreateNewFile();

    // save changes to disk
    void Save(const Data& data);

    // delete the file from disk
    void Delete();

    // read data from the csv file
    [[nodiscard]] Data Read();
};


