#pragma once

#include "../../wxwidgets.h"
#include <string>

class DBCreationDialog : public wxDialog
{
private:
    std::string dbName;
    wxTextCtrl* textCtrl = nullptr;
public:
    DBCreationDialog(wxWindow* parent, wxWindowID id);

    [[nodiscard]] const std::string& GetDbName() const;

private:
    void OnCreate(wxCommandEvent& event);

DECLARE_EVENT_TABLE()
};


