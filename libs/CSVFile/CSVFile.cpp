#include "CSVFile.h"
#include <cstdio>
#include <sstream>
#include <utility>

CSVFile::CSVFile(std::string filename) : filename(std::move(filename)) { }

void CSVFile::createNewFile()
{
    file.open(filename, std::ios::out);
    if (!file.is_open()) throw std::exception("Can't Create file");
}

void CSVFile::close() { if (file.is_open()) file.close(); }

void CSVFile::deleteFile()
{
    close();
    int result = std::remove(filename.c_str());
    // failed to delete
    if (result) throw std::exception("Fail doesn't exist");
}

void CSVFile::createColumns(const Row& columnNames) { data.push_back(columnNames); }

void CSVFile::save()
{
    file.clear();
    file.seekp(0);
    for (const Row& row: data)
    {
        unsigned size = row.size();
        for (int i = 0; i < size; ++i)
        {
            if (i == size - 1) file << row[i];
            else file << row[i] << ",";
        }
        file << "\n";
    }

    file << std::endl;
}

void CSVFile::addRow(const Row& row)
{
    if (data.empty()) throw std::exception("There is no column names");
    if (row.size() < data[0].size())
        throw std::exception("The number of values provided is less than the number of columns");

    data.push_back(row);
}

void CSVFile::read()
{
    file.open(filename, std::ios::in | std::ios::out);
    if (!file.is_open()) throw std::exception("File doesn't exist");

    std::string line, value;
    Row row;
    data.clear();
    while (std::getline(file, line))
    {
        row.clear();
        std::stringstream stream(line);
        while (std::getline(stream, value, ',')) row.push_back(value);
        data.push_back(row);
    }
}