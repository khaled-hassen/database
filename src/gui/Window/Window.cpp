#include "Window.h"
#include <wx/artprov.h>
#include "gui/wxWindowId.h"
#include "gui/Panel/TableSelectionPanel.h"
#include "gui/Panel/RecordsViewPanel.h"
#include "gui/Event/EventHandler.h"

#define BIND_FN_0Param(handler) std::bind(&Window::handler, this)
#define BIND_FN_2Params(handler) std::bind(&Window::handler, this, std::placeholders::_1, std::placeholders::_2)

// statically bind events to functions
BEGIN_EVENT_TABLE(Window, wxFrame)
                EVT_MENU(wxID_EXIT, Window::OnExit)
                EVT_MENU(wxWindowId::OPEN_DB, Window::OnOpenDB)
                EVT_MENU(wxWindowId::NEW_DB, Window::OnCreateDB)
                EVT_MENU(wxWindowId::DROP_DB, Window::OnDropDB)
                EVT_MENU(wxWindowId::NEW_TABLE, Window::OnCreateTable)
                EVT_MENU(wxWindowId::OPEN_TABLE, Window::OnOpenTable)
                EVT_MENU(wxWindowId::DROP_TABLE, Window::OnDropTable)
                EVT_MENU(wxWindowId::DELETE_RECORD, Window::OnDeleteRecord)
                EVT_MENU(wxWindowId::SAVE_TABLE, Window::OnSaveTable)
END_EVENT_TABLE()

Window::Window(const wxString& title, const wxSize& size)
        : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size, wxDEFAULT_FRAME_STYLE, wxFrameNameStr)
{
    // initialize db
    m_Db = Pointer(new Database());

    // initialize the event handler
    m_EventHandler = Pointer(new EventHandler(this, m_Db.GetRowPtr()));

    // create the file menu
    auto* fileMenu = new wxMenu();
    // & underlines the first character, so it can be accessible via the ALT+F shortcut
    fileMenu->Append(wxWindowId::OPEN_DB, "&Open database \tCtrl-O", "Open database");
    fileMenu->Append(wxWindowId::NEW_DB, "&Create database \tCtrl-N", "Create a new database");
    fileMenu->Append(wxWindowId::DROP_DB, "&Delete database \tCtrl-X", "Delete database");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "&Quit \tCtrl-Q");

    // create the menu bar
    auto* menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    wxFrame::SetMenuBar(menuBar);

    // create a status bar
    wxFrame::CreateStatusBar(2);
    wxFrame::SetStatusText("No database is opened", 0);
    wxFrame::SetStatusText("", 1);

    // create the toolbar
    wxFrame::CreateToolBar();

    wxFrame::GetToolBar()->AddTool(wxWindowId::OPEN_DB, "Open database", wxArtProvider::GetBitmap("wxART_FOLDER_OPEN"));
    wxFrame::GetToolBar()->SetToolLongHelp(wxWindowId::OPEN_DB, "Open database");

    wxFrame::GetToolBar()->AddTool(wxWindowId::NEW_DB, "New database", wxArtProvider::GetBitmap("wxART_NEW_DIR"));
    wxFrame::GetToolBar()->SetToolLongHelp(wxWindowId::NEW_DB, "Create a new database");

    wxFrame::GetToolBar()->AddTool(wxWindowId::DROP_DB, "Delete database", wxArtProvider::GetBitmap("wxART_DELETE"));
    wxFrame::GetToolBar()->SetToolLongHelp(wxWindowId::DROP_DB, "Delete database");

    wxFrame::GetToolBar()->Realize();

    // create viewport
    auto* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    m_TablesPanel = new TableSelectionPanel(this, wxWindowId::OPEN_TABLE, wxSize(200, 0));
    mainSizer->Add(m_TablesPanel, 0, wxEXPAND);

    m_RecordsPanel = new RecordsViewPanel(this, wxID_ANY);
    mainSizer->Add(m_RecordsPanel, 1, wxEXPAND);

    SetSizer(mainSizer);
}

void Window::OnExit(wxCommandEvent& event) { Close(true); }

void Window::OnOpenDB(wxCommandEvent& event) { m_EventHandler->OpenDatabase(BIND_FN_0Param(UpdateUIData)); }

void Window::OnCreateDB(wxCommandEvent& event) { m_EventHandler->OnCreateDB(BIND_FN_0Param(UpdateUIData)); }

void Window::OnDropDB(wxCommandEvent& event) { m_EventHandler->OnDropDb(); }

void Window::UpdateUIData()
{
    if (!m_IsTableToolsAdded)
    {
        AddTableTools();
        m_IsTableToolsAdded = true;
    }

    SetStatusText(wxString::Format("Opened database: %s", m_Db->GetDbName().c_str()), 0);
    wxFrame::SetStatusText("", 1);
    const std::vector<std::string>& tables = m_Db->GetTableNames();
    if (m_TablesPanel) m_TablesPanel->ShowTablesList(tables);
}

void Window::AddTableTools()
{
    GetToolBar()->AddTool(wxWindowId::NEW_TABLE, "Create Table", wxArtProvider::GetBitmap("wxART_NEW"));
    GetToolBar()->SetToolLongHelp(wxWindowId::NEW_TABLE, "Create a new table");

    GetToolBar()->AddTool(wxWindowId::DROP_TABLE, "Delete Table", wxArtProvider::GetBitmap("wxART_CROSS_MARK"));
    GetToolBar()->SetToolLongHelp(wxWindowId::DROP_TABLE, "Delete the active table");

    GetToolBar()->Realize();

    wxMenu* menu = GetMenuBar()->GetMenu(0);
    menu->InsertSeparator(3);
    menu->Insert(4, wxWindowId::NEW_TABLE, "&Create table \tCtrl-Shift-N", "Create a new table");
    menu->Insert(5, wxWindowId::DROP_TABLE, "&Delete table \tCtrl-Shift-X", "Delete the active table");
}

void Window::AddRecordTools()
{
    if (m_IsRecordToolsAdded) return;
    m_IsRecordToolsAdded = true;
    GetToolBar()->AddTool(wxWindowId::ADD_RECORD, "Add record", wxArtProvider::GetBitmap("wxART_PLUS"));
    GetToolBar()->SetToolLongHelp(wxWindowId::ADD_RECORD, "Add a new record");

    GetToolBar()->AddTool(wxWindowId::DELETE_RECORD, "Remove record", wxArtProvider::GetBitmap("wxART_MINUS"));
    GetToolBar()->SetToolLongHelp(wxWindowId::DELETE_RECORD, "Remove the selected record");

    GetToolBar()->AddStretchableSpace();
    GetToolBar()->AddTool(wxWindowId::SAVE_TABLE, "Save table", wxArtProvider::GetBitmap("wxART_FILE_SAVE"));
    GetToolBar()->SetToolLongHelp(wxWindowId::SAVE_TABLE, "Save table changes");

    GetToolBar()->Realize();

    wxMenu* menu = GetMenuBar()->GetMenu(0);
    menu->Insert(6, wxWindowId::SAVE_TABLE, "&Save table \tCtrl-S", "Save table changes");
    menu->InsertSeparator(7);
    menu->Insert(8, wxWindowId::ADD_RECORD, "&Add record \tCtrl-A", "Add a new record");
    menu->Insert(9, wxWindowId::DELETE_RECORD, "&Remove record \tCtrl-D", "Remove the selected record");
}

void Window::OnCreateTable(wxCommandEvent& event) { m_EventHandler->OnCreateTable(BIND_FN_2Params(UpdateTableUI)); }

void Window::UpdateTableUI(const std::string& tableName, bool update)
{
    if (update) UpdateUIData();
    UpdateTableViewUI(tableName);
    AddRecordTools();
}

void Window::OnOpenTable(wxCommandEvent& event)
{
    if (m_TablesPanel == nullptr) return;
    const std::string& tableName = m_TablesPanel->GetTableName();
    m_Db->OpenTable(tableName);
    UpdateTableUI(tableName, false);
}

void Window::UpdateTableViewUI(const std::string& tableName)
{
    SetStatusText(wxString::Format("Active table: %s", tableName.c_str()), 1);
    if (m_RecordsPanel == nullptr) return;
    m_RecordsPanel->ShowRecords(m_Db->GetTable()->GetData(), m_Db->GetTable()->GetColumns());
}

void Window::OnDropTable(wxCommandEvent& event)
{
    m_EventHandler->OnDropTable(BIND_FN_0Param(UpdateUIData));
    if (m_RecordsPanel == nullptr) return;
    m_RecordsPanel->ClearRecords();
}

void Window::OnSaveTable(wxCommandEvent& event) { m_Db->GetTable()->Save(); }

void Window::OnDeleteRecord(wxCommandEvent& event)
{
    if (m_RecordsPanel == nullptr) return;
    long index = m_RecordsPanel->GetSelectedRecord();
    m_EventHandler->OnDeleteRecord(index);
    // show the updated records
    m_RecordsPanel->ShowRecords(m_Db->GetTable()->GetData(), m_Db->GetTable()->GetColumns());
}
