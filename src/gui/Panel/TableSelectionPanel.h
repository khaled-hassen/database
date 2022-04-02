#pragma once

#include "../wxwidgets.h"
#include <string>
#include <vector>

class TableSelectionPanel : public wxPanel
{
private:
    class wxListView* tableList = nullptr;

    std::string tableName;

public:
    TableSelectionPanel(wxWindow* parent, wxWindowID id, const wxSize& size);

    [[nodiscard]] const std::string& GetTableName() const;

    // display the tables list to the ui
    void ShowTablesList(const std::vector<std::string>& items);

private:
    void OnSelectTable(class wxListEvent& event);

DECLARE_EVENT_TABLE()
};


