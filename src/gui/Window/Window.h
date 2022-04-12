#pragma once

#include "gui/wxwidgets.h"
#include "Database/Database.h"
#include "gui/Event/EventHandler.h"

class Window : public wxFrame
{
private:
    bool m_IsTableToolsAdded = false;
    bool m_IsRecordToolsAdded = false;
    Pointer<Database> m_Db;
    Pointer<EventHandler> m_EventHandler;

    class TableSelectionPanel* m_TablesPanel = nullptr;

    class RecordsViewPanel* m_RecordsPanel = nullptr;

public:
    Window(const wxString& title, const wxSize& size);

private:
    // update the ui with the new data
    void UpdateUIData();

    // update the table view ui
    void UpdateTableUI(const std::string& tableName, bool update);

    void AddTableTools();

    void AddRecordTools();

    // handle close event
    void OnExit(wxCommandEvent& event);

    void OnOpenDB(wxCommandEvent& event);

    void OnCreateDB(wxCommandEvent& event);

    void OnDropDB(wxCommandEvent& event);

    void OnCreateTable(wxCommandEvent& event);

    void OnOpenTable(wxCommandEvent& event);

    void UpdateTableViewUI(const std::string& tableName);

    void OnDropTable(wxCommandEvent& event);

    void OnSaveTable(wxCommandEvent& event);

    void OnDeleteRecord(wxCommandEvent& event);

// for binding events (must be at the end of the class: may change class visibility)
DECLARE_EVENT_TABLE()

};


