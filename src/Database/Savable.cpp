#include "Savable.h"
#include <filesystem>

#define CHAR_CAST(data) (reinterpret_cast<char*>(&(data)))

Savable::Savable(std::string filename) : m_Filename(std::move(filename)) { }

void Savable::CreateFile()
{
    m_File.open(m_Filename, std::ios::out | std::ios::binary);
    if (!m_File.is_open() || m_File.fail()) throw std::exception("Can't create file");
}

void Savable::OpenFile()
{
    m_File.open(m_Filename, std::ios::in | std::ios::binary);
    if (!m_File.is_open() || m_File.fail()) throw std::exception("Can't open file");
}

void Savable::WriteSize(size_t size) { m_File.write(CHAR_CAST(size), sizeof(size_t)); }

void Savable::WriteString(const std::string& str)
{
    size_t size = str.size();
    WriteSize(size);
    m_File.write(str.c_str(), size);
}

size_t Savable::ReadSize()
{
    size_t size = 0;
    m_File.read(CHAR_CAST(size), sizeof(size_t));
    return size;
}

std::string Savable::ReadString()
{
    size_t size = ReadSize();
    char* buffer = new char[size + 1];
    m_File.read(buffer, size);
    buffer[size] = '\0';
    std::string str = buffer;
    delete[] buffer;
    return str;
}

void Savable::Delete()
{
    CloseFile();
    bool result = std::filesystem::remove(m_Filename);
    // failed to delete
    if (!result) throw std::exception("File doesn't exist");
}
