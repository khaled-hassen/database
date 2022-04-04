#pragma once

#include "../wxwidgets.h"
#include "../../Database/Database.h"

class Window : public wxFrame
{
private:
    bool m_IsTableToolAdded = false;
    Pointer<Database> m_Db;
    wxToolBar* m_Toolbar = nullptr;

    class TableSelectionPanel* m_LeftPanel = nullptr;

    class RecordsViewPanel* m_RightPanel = nullptr;

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

    void OnOpenTable(wxCommandEvent& event);

    void OnDropTable(wxCommandEvent& event);

// for binding events (must be at the end of the class: may change class visibility)
DECLARE_EVENT_TABLE()
};


