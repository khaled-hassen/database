#pragma once

#include "../wxwidgets.h"
#include <string>

class DBSelectionDialog : public wxDialog
{
private:
    std::string dbName;

public:
    DBSelectionDialog(wxWindow* parent, wxWindowID id);

    [[nodiscard]] const std::string& GetDbName() const;

private:
    void OnSelectItem(class wxListEvent& event);

    void OnItemSelected(class wxListEvent& event);

DECLARE_EVENT_TABLE()
};


