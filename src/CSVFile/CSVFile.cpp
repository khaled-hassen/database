#include "CSVFile.h"
#include <sstream>
#include <utility>
#include <filesystem>

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
    bool result = std::filesystem::remove(filename);
    // failed to delete
    if (!result) throw std::exception("File doesn't exist");
}


void CSVFile::Save(const Columns& cols, const Data& data)
{
    file.clear();
    file.seekp(0);
    unsigned size = cols.size();
    for (const auto& col: cols) file << col << ",";
    // delete trailing comma
    long long pos = file.tellp();
    file.seekp(pos - 1);
    file.write(" ", 1);
    file << std::endl;

    for (const auto& record: data)
    {
        for (const auto& it: record) file << it.second << ",";
        // delete trailing comma
        pos = file.tellp();
        file.seekp(pos - 1);
        file.write(" ", 1);
        file << std::endl;
    }
}

void CSVFile::Read(Columns& outCols, Data& outData)
{
    file.open(filename, std::ios::in);
    if (!file.is_open()) throw std::exception("File doesn't exist");

    std::string line, value;
    outCols.clear();
    outData.clear();
    bool isFirstLine = true;
    while (std::getline(file, line))
    {
        std::stringstream stream(line);
        if (isFirstLine)
        {
            isFirstLine = false;
            while (std::getline(stream, value, ',')) outCols.push_back(value);
        }
        else
        {
            Record record;
            record.reserve(outCols.size());
            unsigned idx = 0;
            while (std::getline(stream, value, ','))
            {
                std::string col = outCols.at(idx++);
                unsigned index = col.find(':');
                std::string key = col.substr(0, index);
                record.insert({ key, value });
            }
            outData.push_back(record);
        }
    }
}
