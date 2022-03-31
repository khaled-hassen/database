#pragma once

#include "../wxwidgets.h"
#include "../../Database/Database.h"

class Window : public wxFrame
{
private:
    Pointer<Database> db;

public:
    Window(const wxString& name, const wxSize& size);

private:
    // handle close event
    void OnExit(wxCommandEvent& event);

    void OnListItemSelected(class wxListEvent& event);

    // for binding events (must be at the end of the class: may change class visibility)
    DECLARE_EVENT_TABLE()
};


