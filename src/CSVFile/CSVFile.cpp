#include "CSVFile.h"
#include <sstream>
#include <utility>
#include <filesystem>
#include "Utils/String/StringUtils.h"

CSVFile::CSVFile(std::string filename) : m_Filename(std::move(filename)) { }

CSVFile::~CSVFile() { Close(); }

void CSVFile::CreateNewFile()
{
    m_File.open(m_Filename, std::ios::out);
    if (!m_File.is_open()) throw std::exception("Can't create file");
}

void CSVFile::Close() { if (m_File.is_open()) m_File.close(); }

void CSVFile::Delete()
{
    Close();
    bool result = std::filesystem::remove(m_Filename);
    // failed to delete
    if (!result) throw std::exception("File doesn't exist");
}


void CSVFile::Save(const Columns& cols, const Data& data)
{
    m_File.clear();
    m_File.seekp(0);
    for (auto col: cols)
    {
        StringUtils::Replace(col, ' ', '-');
        m_File << col << ",";
    }
    // delete trailing comma
    long long pos = m_File.tellp();
    m_File.seekp(pos - 1);
    m_File.write(" ", 1);
    m_File << std::endl;

    for (const auto& record: data)
    {
        for (const auto&[_, val]: record) m_File << val << ",";
        // delete trailing comma
        pos = m_File.tellp();
        m_File.seekp(pos - 1);
        m_File.write(" ", 1);
        m_File << std::endl;
    }
}

void CSVFile::Read(Columns& outCols, Data& outData)
{
    m_File.open(m_Filename, std::ios::in);
    if (!m_File.is_open()) throw std::exception("File doesn't exist");

    std::string line, value;
    outCols.clear();
    outData.clear();
    bool isFirstLine = true;
    while (std::getline(m_File, line))
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
