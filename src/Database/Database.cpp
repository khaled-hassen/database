#include "Database.h"
#include "Utils/StringUtils.h"
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;
std::string Database::s_Root = "data/";
std::string Database::s_Extension = ".db";
std::string Database::s_TableExtension = ".tb";

Database::Database(const std::string& name) : Savable(s_Root + name + s_Extension), m_Name(name) { }

std::vector<std::string> Database::ScanForDatabases()
{
    if (!fs::exists(s_Root)) return std::vector<std::string>(0);

    std::vector<std::string> dbNames;
    for (const auto& entry: fs::directory_iterator(s_Root))
    {
        if (entry.path().extension() != s_Extension) continue;
        std::string filename = entry.path().string();
        dbNames.push_back(filename.substr(s_Root.length(), filename.length() - s_Root.length() - s_Extension.length()));
    }

    return dbNames;
}

void Database::CreateDb()
{
    CloseDb();
    if (!fs::exists(s_Root))
    {
        bool success = fs::create_directory(s_Root);
        if (!success) throw std::exception("Can't create database");
    }
    if (fs::exists(GetPath())) throw std::exception("Database exists");

    m_Open = true;
    Save();
}

void Database::OpenDb()
{
    CloseDb();
    CheckDbExists();
    m_Open = true;
    Load();
}

void Database::CheckOpenedDb() const { if (!m_Open) throw std::exception("There is no opened database"); }

void Database::CheckDbExists() { if (!fs::exists(GetPath())) throw std::exception("Database doesn't exist"); }

void Database::CloseDb()
{
    m_TableNames.clear();
    m_Open = false;
    CloseTable();
}

void Database::DropDb(const std::string& dbName)
{
    Database db(dbName);
    db.OpenDb();
    for (const auto& table: db.m_TableNames)
        Table(CreateTablePath(table), table).Delete();
    db.Delete();
}

std::string Database::CreateTablePath(const std::string& tableName)
{
    return s_Root + tableName + s_TableExtension;
}

void Database::CreateTable(const std::string& tableName, const Columns& cols)
{
    CheckOpenedDb();
    CloseTable();
    const std::string& path = CreateTablePath(tableName);
    if (fs::exists(path)) throw std::exception("Table exists");

    m_Table = Pointer(new Table(path, tableName, cols));
    m_Table->Save();
    m_TableNames.push_back(tableName);
    Save();
}

void Database::OpenTable(const std::string& tableName)
{
    CheckOpenedDb();
    CloseTable();
    const std::string& path = CreateTablePath(tableName);
    m_Table = Pointer(new Table(path, tableName));
    m_Table->Load();
}

const Pointer<Table>& Database::GetTable() const
{
    CheckOpenedTable();
    return m_Table;
}

void Database::CheckOpenedTable() const
{
    CheckOpenedDb();
    if (m_Table == nullptr) throw std::exception("There's no opened table");
}

void Database::CloseTable() { m_Table = Pointer<Table>::GetNull(); }

void Database::DropTable()
{
    CheckOpenedDb();
    std::string name = GetTable()->GetName();
    m_Table->Delete();
    CloseTable();
    // remove the tableName from the tableNames vector
    m_TableNames.erase(std::remove(m_TableNames.begin(), m_TableNames.end(), name), m_TableNames.end());
    Save();
}

void Database::Load()
{
    OpenFile();
    size_t size = ReadSize();
    m_TableNames.resize(size);
    for (auto& table: m_TableNames) table = ReadString();
    CloseFile();
}

void Database::Save()
{
    CreateFile();
    WriteSize(m_TableNames.size());
    for (const auto& table: m_TableNames) WriteString(table);
    CloseFile();
}
