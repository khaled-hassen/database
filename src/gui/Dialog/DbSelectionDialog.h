#pragma once

#include "gui/wxwidgets.h"

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

// for binding events (must be at the end of the class: may change class visibility)
DECLARE_EVENT_TABLE()
};


