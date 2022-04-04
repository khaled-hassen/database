#pragma once

#include "../../wxwidgets.h"
#include <string>

class DBSelectionDialog : public wxDialog
{
private:
    std::string m_DbName;

public:
    DBSelectionDialog(wxWindow* parent, wxWindowID id, const wxString& title);

    [[nodiscard]] inline const std::string& GetDbName() const { return m_DbName; }

private:
    void OnSelectItem(class wxListEvent& event);

    void OnItemSelected(class wxListEvent& event);

DECLARE_EVENT_TABLE()
};


