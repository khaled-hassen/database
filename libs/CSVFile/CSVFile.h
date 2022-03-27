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
public:
    explicit CSVFile(std::string filename);

    // create a new csv file
    void CreateNewFile();

    // save changes to disk
    void Save(const Data& data);

    // close the file
    void Close();

    // delete the file from disk
    void DeleteFile();

    // read data from the csv file
    void Read();
};


