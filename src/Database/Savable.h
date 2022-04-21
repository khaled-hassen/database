#pragma once

#include <string>
#include <utility>
#include <fstream>

class Savable
{
private:
    std::string m_Filename;
    std::fstream m_File;

public:
    explicit Savable(std::string filename);

    [[nodiscard]] inline const std::string& GetPath() const { return m_Filename; }

    inline void CloseFile() { m_File.close(); }

    // create a new file with the name passed to the constructor
    void CreateFile();

    // open file with the name passed to the constructor
    void OpenFile();

    // delete the file with the name passed to the constructor from disk
    void Delete();

    // write size_t value to a binary file
    void WriteSize(size_t size);

    // write a std::string value to a binary file
    void WriteString(const std::string& str);

    // read size_t value from a binary file and return it
    size_t ReadSize();

    // read std::string value from a binary file and return it
    std::string ReadString();

    virtual void Load() = 0;

    virtual void Save() = 0;
};
