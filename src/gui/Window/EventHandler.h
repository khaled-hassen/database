#pragma once

#include "Utils/Pointer.h"
#include <functional>
#include <wx/event.h>

using UpdateUIFn = std::function<void()>;
using UpdateTableUIFn = std::function<void(const std::string&, bool)>;

class Database;

class wxFrame;

class RecordsViewPanel;

class EventHandler
{
private:
    Database* m_Db = nullptr;
    wxFrame* m_Parent = nullptr;
    RecordsViewPanel* m_RecordsPanel = nullptr;

public:
    EventHandler(wxFrame* parent, Database* db, RecordsViewPanel* recordsPanel);

    void UpdateRecordsView() const;

    void OpenDatabase(const UpdateUIFn& updateUI) const;

    void CreateDB(const UpdateUIFn& updateUI) const;

    void DropDB() const;

    void OpenTable(class TablesSelectionPanel* tablesPanel, const UpdateTableUIFn& updateTableUI) const;

    void CreateTable(const UpdateTableUIFn& updateTableUI) const;

    void SaveTable() const;

    void DropTable(const UpdateUIFn& updateUI) const;

    void AddRecord() const;

    void EditRecord() const;

    void DeleteRecord() const;

};