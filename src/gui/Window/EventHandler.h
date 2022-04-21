#pragma once

#include "Utils/Pointer.h"

class Window;

class EventHandler
{
private:
    Pointer<class Database> m_Database;
    Window* m_Parent = nullptr;

public:
    explicit EventHandler(Window* parent);

    [[nodiscard]] inline const Database* GetDatabase() const { return m_Database.GetRawPtr(); }

    void OpenDatabase();

    void CreateDB();

    void DropDB() const;

    void OpenTable();

    void CreateTable();

    void SaveTable() const;

    void DropTable() const;

    void AddRecord() const;

    void EditRecord() const;

    void DeleteRecord() const;

private:
    // update the table view ui
    void UpdateTableUI(const std::string& tableName, bool update);

    void UpdateRecordsView() const;
};