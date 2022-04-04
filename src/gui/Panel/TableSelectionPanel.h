#pragma once

#include "gui/wxwidgets.h"
#include <string>
#include <vector>

class TableSelectionPanel : public wxPanel
{
private:
    class wxListView* m_TableList = nullptr;

    std::string m_TableName;

public:
    TableSelectionPanel(wxWindow* parent, wxWindowID id, const wxSize& size);

    [[nodiscard]] inline const std::string& GetTableName() const { return m_TableName; }

    // display the tables list to the ui
    void ShowTablesList(const std::vector<std::string>& items);

private:
    void OnSelectTable(class wxListEvent& event);

DECLARE_EVENT_TABLE()
};


