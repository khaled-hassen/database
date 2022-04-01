#pragma once

#include "../wxwidgets.h"
#include <string>
#include <vector>

class TableSelectionPanel : public wxPanel
{
private:
    class wxListView* tableList = nullptr;

public:
    TableSelectionPanel(wxWindow* parent, wxWindowID id, const wxSize& size);

    // display the tables list to the ui
    void ShowTablesList(const std::vector<std::string>& items);
};


