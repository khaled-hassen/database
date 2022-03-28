#include "CSVFile.h"
#include <cstdio>
#include <sstream>
#include <utility>

CSVFile::CSVFile(std::string filename) : filename(std::move(filename)) { }

CSVFile::~CSVFile() { Close(); }

void CSVFile::CreateNewFile()
{
    file.open(filename, std::ios::out);
    if (!file.is_open()) throw std::exception("Can't create file");
}

void CSVFile::Close() { if (file.is_open()) file.close(); }

void CSVFile::Delete()
{
    Close();
    int result = std::remove(filename.c_str());
    // failed to delete
    if (result) throw std::exception("Fail doesn't exist");
}


void CSVFile::Save(const Data& data)
{
    file.clear();
    file.seekp(0);
    for (const auto& row: data)
    {
        unsigned size = row.size();
        for (int i = 0; i < size; ++i)
        {
            if (i == size - 1) file << row[i];
            else file << row[i] << ",";
        }
        file << std::endl;
    }
}

Data CSVFile::Read()
{
    file.open(filename, std::ios::in | std::ios::out);
    if (!file.is_open()) throw std::exception("File doesn't exist");

    std::string line, value;
    Data data;
    std::vector<std::string> row;
    while (std::getline(file, line))
    {
        row.clear();
        std::stringstream stream(line);
        while (std::getline(stream, value, ',')) row.push_back(value);
        data.push_back(row);
    }

    return data;
}
