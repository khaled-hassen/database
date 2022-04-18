#pragma once

#include "gui/wxwidgets.h"
#include "Database/Database.h"
#include "EventHandler.h"

class TablesSelectionPanel;

class RecordsViewPanel;

class Window : public wxFrame
{
private:
    bool m_IsTableToolsAdded = false;
    bool m_IsRecordToolsAdded = false;
    Pointer<Database> m_Db;
    Pointer<EventHandler> m_EventHandler;
    TablesSelectionPanel* m_TablesPanel = nullptr;
    RecordsViewPanel* m_RecordsPanel = nullptr;

public:
    Window(const wxString& title, const wxSize& size);

private:
    // update the ui with the new data
    void UpdateUIData();

    // update the table view ui
    void UpdateTableUI(const std::string& tableName, bool update);

    void AddTableTools();

    void AddRecordTools();

};


