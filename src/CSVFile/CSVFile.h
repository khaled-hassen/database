#pragma once

#include "Utils/types/types.h"
#include <fstream>
#include <string>

class CSVFile
{
private:
    std::string m_Filename;
    std::fstream m_File;

private:
    // close the file
    void Close();

public:
    explicit CSVFile(std::string filename);

    virtual ~CSVFile();

    // create a new csv file
    void CreateNewFile();

    // save changes to disk
    void Save(const Columns& cols, const Data& data);

    // delete the file from disk
    void Delete();

    // read data from the csv file
    void Read(Columns& outCols, Data& outData);
};


