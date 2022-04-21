#pragma once

#include "gui/wxwidgets.h"

class DBCreationDialog : public wxDialog
{
private:
    std::string m_DbName;
    wxTextCtrl* m_TextCtrl = nullptr;
public:
    DBCreationDialog(wxWindow* parent, wxWindowID id);

    [[nodiscard]] inline const std::string& GetDbName() const { return m_DbName; }

private:
    void OnCreate(wxCommandEvent& event);

DECLARE_EVENT_TABLE()
};


