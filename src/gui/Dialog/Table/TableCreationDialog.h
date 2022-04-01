#pragma once

#include "../../wxwidgets.h"
#include <string>

class TableCreationDialog : public wxDialog
{
private:
    std::string tableName;

    class wxTextCtrl* textCtrl = nullptr;

public:
    TableCreationDialog(wxWindow* parent, wxWindowID id);

    const std::string& GetTableName() const;

private:
    void OnCreate(wxCommandEvent& event);

DECLARE_EVENT_TABLE()
};


