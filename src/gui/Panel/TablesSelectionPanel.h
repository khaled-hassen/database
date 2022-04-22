#pragma once

#include "gui/wxwidgets.h"

class wxListView;

class TablesSelectionPanel : public wxPanel
{
private:
    wxListView* m_TableList = nullptr;
    std::string m_TableName;
    long m_TableIndex = -1;
    bool m_HasUnsavedChanges = false;

public:
    TablesSelectionPanel(wxWindow* parent, wxWindowID id, const wxSize& size);

    [[nodiscard]] inline const std::string& GetTableName() const { return m_TableName; }

    [[nodiscard]] inline long GetTableIndex() const { return m_TableIndex; }

    // display the tables list to the ui
    void ShowTablesList(const std::vector<std::string>& items);

    void ClearTables();

    void SetTableSaveState(long index, bool unsaved);

    // return true if table is table can be closed otherwise return false
    bool CanCloseTable();

private:
    void OnSelectTable(class wxListEvent& event);

DECLARE_EVENT_TABLE()
};


