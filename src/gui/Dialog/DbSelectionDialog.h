#pragma once

#include "gui/wxwidgets.h"
#include <string>
#include <wx/listbase.h>

class DBSelectionDialog : public wxDialog
{
private:
    std::string m_DbName;

public:
    DBSelectionDialog(wxWindow* parent, wxWindowID id, const wxString& title);

    [[nodiscard]] inline const std::string& GetDbName() const { return m_DbName; }

private:
    void OnSelectItem(class wxListEvent& event);

    void OnItemActivated(wxListEvent& event);

DECLARE_EVENT_TABLE()
};


