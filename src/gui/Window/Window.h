#pragma once

#include "../wxwidgets.h"
#include "../../Database/Database.h"

class Window : public wxFrame
{
private:
    Pointer<Database> db;

    class TableSelectionPanel* leftPanel = nullptr;

    class RecordsViewPanel* rightPanel = nullptr;

    wxToolBar* toolbar = nullptr;

public:
    Window(const wxString& title, const wxSize& size);

private:
    void UpdateUI();

    // handle close event
    void OnExit(wxCommandEvent& event);

    void OnOpenDB(wxCommandEvent& event);

    void OnCreateDB(wxCommandEvent& event);

    void OnCreateTable(wxCommandEvent& event);

// for binding events (must be at the end of the class: may change class visibility)
DECLARE_EVENT_TABLE()
};


