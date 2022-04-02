#pragma once

#include "../wxwidgets.h"
#include "../../Database/Database.h"

class Window : public wxFrame
{
private:
    bool isTableToolAdded = false;
    Pointer<Database> db;
    wxToolBar* toolbar = nullptr;

    class TableSelectionPanel* leftPanel = nullptr;

    class RecordsViewPanel* rightPanel = nullptr;

public:
    Window(const wxString& title, const wxSize& size);

private:
    void UpdateUI();

    // handle close event
    void OnExit(wxCommandEvent& event);

    void OnOpenDB(wxCommandEvent& event);

    void OnCreateDB(wxCommandEvent& event);

    void OnDropDB(wxCommandEvent& event);

    void OnCreateTable(wxCommandEvent& event);

// for binding events (must be at the end of the class: may change class visibility)
DECLARE_EVENT_TABLE()
};


