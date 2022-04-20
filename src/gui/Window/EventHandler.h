#pragma once

#include "Utils/Pointer.h"
#include <functional>
#include <wx/event.h>
#include "Utils/Pointer.h"
#include "Database/Database.h"

using UpdateUIFn = std::function<void()>;
using UpdateTableUIFn = std::function<void(const std::string&, bool)>;

class wxFrame;

class RecordsViewPanel;

class EventHandler
{
private:
    Pointer<Database> m_Database;
    wxFrame* m_Parent = nullptr;
    RecordsViewPanel* m_RecordsPanel = nullptr;

public:
    EventHandler(wxFrame* parent, RecordsViewPanel* recordsPanel);

    [[nodiscard]] inline const Database* GetDatabase() const { return m_Database.GetRawPtr(); }

    void UpdateRecordsView() const;

    void OpenDatabase(const UpdateUIFn& updateUI);

    void CreateDB(const UpdateUIFn& updateUI);

    void DropDB() const;

    void OpenTable(class TablesSelectionPanel* tablesPanel, const UpdateTableUIFn& updateTableUI) const;

    void CreateTable(const UpdateTableUIFn& updateTableUI) const;

    void SaveTable() const;

    void DropTable(const UpdateUIFn& updateUI) const;

    void AddRecord() const;

    void EditRecord() const;

    void DeleteRecord() const;
};