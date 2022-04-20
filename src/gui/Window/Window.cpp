#include "Window.h"
#include <wx/artprov.h>
#include "gui/wxWindowId.h"
#include "gui/Panel/TablesSelectionPanel.h"
#include "gui/Panel/RecordsViewPanel.h"

// these macros bind Window class methods and return std::function with the appropriate arguments
#define BIND_FN(handler) std::bind(&Window::handler, this)
#define BIND_FN_2Params(handler) std::bind(&Window::handler, this, std::placeholders::_1, std::placeholders::_2)

// these macros bind wxWidgets event to the appropriate EventHandler method
#define BIND_EVENT(eventID, handler) Bind(wxEVT_MENU, [this](wxCommandEvent&) \
    { \
        std::bind(&handler, m_EventHandler.GetRawPtr())(); \
    }, eventID)

#define BIND_EVENT_1Param(eventID, handler, arg) Bind(wxEVT_MENU, [this](wxCommandEvent&) \
    { \
        std::bind(&handler, m_EventHandler.GetRawPtr(), std::placeholders::_1)(arg); \
    }, eventID)

#define BIND_EVENT_2Params(eventID, handler, arg1, arg2) Bind(wxEVT_MENU, [this](wxCommandEvent&) \
    { \
        std::bind(&handler, m_EventHandler.GetRawPtr(), std::placeholders::_1, std::placeholders::_2) \
        (arg1, arg2); \
    }, eventID)

// add a new tool in the toolbar
#define ADD_TOOL(id, title, description, icon) \
    wxFrame::GetToolBar()->AddTool(id, title, wxArtProvider::GetBitmap(icon));\
    wxFrame::GetToolBar()->SetToolLongHelp(id, description)

Window::Window(const wxString& title, const wxSize& size)
        : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size, wxDEFAULT_FRAME_STYLE, wxFrameNameStr)
{
    // create the file menu
    auto* fileMenu = new wxMenu();
    // & underlines the first character, so it can be accessible via the ALT+F shortcut
    fileMenu->Append(wxID_EXIT, "&Quit \tCtrl-Q");

    auto* dbMenu = new wxMenu();
    dbMenu->Append(wxWindowId::OPEN_DB, "&Open database \tCtrl-O", "Open database");
    dbMenu->Append(wxWindowId::NEW_DB, "&Create database \tCtrl-N", "Create a new database");
    dbMenu->Append(wxWindowId::DROP_DB, "&Delete database \tCtrl-X", "Delete database");

    auto* menuBar = new wxMenuBar();
    // create the menu bar
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(dbMenu, "&Database");
    wxFrame::SetMenuBar(menuBar);

    // create a status bar
    wxFrame::CreateStatusBar(2);
    wxFrame::SetStatusText("No database is opened", 0);
    wxFrame::SetStatusText("", 1);

    // create the toolbar
    wxFrame::CreateToolBar();
    ADD_TOOL(wxWindowId::OPEN_DB, "Open database", "Open database", "wxART_FOLDER_OPEN");
    ADD_TOOL(wxWindowId::NEW_DB, "New database", "Create a new database", "wxART_NEW_DIR");
    ADD_TOOL(wxWindowId::DROP_DB, "Delete database", "Delete database", "wxART_DELETE");
    wxFrame::GetToolBar()->Realize();

    // create viewport
    auto* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    m_TablesPanel = new TablesSelectionPanel(this, wxWindowId::OPEN_TABLE, wxSize(200, 0));
    mainSizer->Add(m_TablesPanel, 0, wxEXPAND);

    m_RecordsPanel = new RecordsViewPanel(this, wxWindowId::EDIT_RECORD);
    mainSizer->Add(m_RecordsPanel, 1, wxEXPAND);
    SetSizer(mainSizer);

    // initialize the event handler
    m_EventHandler = Pointer(new EventHandler(this, m_RecordsPanel));

    // Bind event to event EventHandler instance
    BIND_EVENT_1Param(wxWindowId::OPEN_DB, EventHandler::OpenDatabase, BIND_FN(UpdateUIData));
    BIND_EVENT_1Param(wxWindowId::NEW_DB, EventHandler::CreateDB, BIND_FN(UpdateUIData));
    BIND_EVENT(wxWindowId::DROP_DB, EventHandler::DropDB);
    BIND_EVENT_2Params(wxWindowId::OPEN_TABLE, EventHandler::OpenTable, m_TablesPanel, BIND_FN_2Params(UpdateTableUI));
    BIND_EVENT_1Param(wxWindowId::NEW_TABLE, EventHandler::CreateTable, BIND_FN_2Params(UpdateTableUI));
    BIND_EVENT(wxWindowId::SAVE_TABLE, EventHandler::SaveTable);
    BIND_EVENT_1Param(wxWindowId::DROP_TABLE, EventHandler::DropTable, BIND_FN(UpdateUIData));
    BIND_EVENT(wxWindowId::ADD_RECORD, EventHandler::AddRecord);
    BIND_EVENT(wxWindowId::EDIT_RECORD, EventHandler::EditRecord);
    BIND_EVENT(wxWindowId::DELETE_RECORD, EventHandler::DeleteRecord);

    Bind(wxEVT_MENU, [this](wxCommandEvent&) { Close(true); }, wxID_EXIT);
}

void Window::UpdateUIData()
{
    if (!m_IsTableToolsAdded)
    {
        AddTableTools();
        m_IsTableToolsAdded = true;
    }

    const Database* db = m_EventHandler->GetDatabase();
    SetStatusText(wxString::Format("Opened database: %s", db->GetName().c_str()), 0);
    wxFrame::SetStatusText("", 1);
    const std::vector<std::string>& tables = db->GetTableNames();
    if (m_TablesPanel) m_TablesPanel->ShowTablesList(tables);
}

void Window::AddTableTools()
{
    ADD_TOOL(wxWindowId::NEW_TABLE, "Create Table", "Create a new table", "wxART_NEW");
    ADD_TOOL(wxWindowId::DROP_TABLE, "Delete Table", "Delete the active table", "wxART_CROSS_MARK");
    GetToolBar()->Realize();

    auto* tableMenu = new wxMenu();
    tableMenu->Append(wxWindowId::NEW_TABLE, "&Create table \tCtrl-Shift-N", "Create a new table");
    tableMenu->Append(wxWindowId::DROP_TABLE, "&Delete table \tCtrl-Shift-X", "Delete the active table");
    GetMenuBar()->Append(tableMenu, "&Table");
}

void Window::AddRecordTools()
{
    if (m_IsRecordToolsAdded) return;
    m_IsRecordToolsAdded = true;

    ADD_TOOL(wxWindowId::ADD_RECORD, "Add record", "Add a new record", "wxART_PLUS");
    ADD_TOOL(wxWindowId::DELETE_RECORD, "Remove record", "Remove the selected record", "wxART_MINUS");
    GetToolBar()->AddStretchableSpace();
    ADD_TOOL(wxWindowId::SAVE_TABLE, "Save table", "Save table changes", "wxART_FILE_SAVE");
    GetToolBar()->Realize();

    // Add a save action to table menu
    GetMenuBar()->GetMenu(2)->Append(wxWindowId::SAVE_TABLE, "&Save table \tCtrl-S", "Save table changes");

    auto* recordMenu = new wxMenu();
    recordMenu->Append(wxWindowId::ADD_RECORD, "&Add record \tCtrl-A", "Add a new record");
    recordMenu->Append(wxWindowId::DELETE_RECORD, "&Remove record \tCtrl-D", "Remove the selected record");
    GetMenuBar()->Append(recordMenu, "&Record");
}

void Window::UpdateTableUI(const std::string& tableName, bool update)
{
    if (update) UpdateUIData();

    SetStatusText(wxString::Format("Active table: %s", tableName.c_str()), 1);
    m_EventHandler->UpdateRecordsView();
    AddRecordTools();
}
