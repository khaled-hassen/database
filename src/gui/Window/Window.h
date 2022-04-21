#pragma once

#include "gui/wxwidgets.h"
#include "Utils/Pointer.h"

class TablesSelectionPanel;

class RecordsViewPanel;

class Window : public wxFrame
{
private:
    bool m_IsTableToolsAdded = false;
    bool m_IsRecordToolsAdded = false;
    Pointer<class EventHandler> m_EventHandler;
    TablesSelectionPanel* m_TablesPanel = nullptr;
    RecordsViewPanel* m_RecordsPanel = nullptr;

public:
    Window(const wxString& title, const wxSize& size);

    // update the ui with the new data
    void UpdateUIData();

    void ClearTableView();

    [[nodiscard]] RecordsViewPanel* GetRecordsPanel() const { return m_RecordsPanel; }

    [[nodiscard]] TablesSelectionPanel* GetTablesPanel() const { return m_TablesPanel; }

    void AddRecordTools();

private:
    void AddTableTools();
};


