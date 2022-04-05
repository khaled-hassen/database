#include "Window.h"
#include <wx/artprov.h>
#include "gui/wxWindowId.h"
#include "gui/Dialog/Database/DbSelectionDialog.h"
#include "gui/Dialog/Database/DbCreationDialog.h"
#include "gui/Panel/TableSelectionPanel.h"
#include "gui/Panel/RecordsViewPanel.h"
#include "gui/Dialog/Table/TableCreationDialog.h"

// statically bind events to functions
BEGIN_EVENT_TABLE(Window, wxFrame)
                EVT_MENU(wxID_EXIT, Window::OnExit)
                EVT_MENU(wxWindowId::OPEN_DB, Window::OnOpenDB)
                EVT_MENU(wxWindowId::NEW_DB, Window::OnCreateDB)
                EVT_MENU(wxWindowId::DROP_DB, Window::OnDropDB)
                EVT_MENU(wxWindowId::NEW_TABLE, Window::OnCreateTable)
                EVT_MENU(wxWindowId::OPEN_TABLE, Window::OnOpenTable)
                EVT_MENU(wxWindowId::DROP_TABLE, Window::OnDropTable)
END_EVENT_TABLE()

Window::Window(const wxString& title, const wxSize& size)
        : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size, wxDEFAULT_FRAME_STYLE, wxFrameNameStr)
{
    // initialize db
    m_Db = Pointer(new Database());

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
    m_Toolbar = wxFrame::CreateToolBar();

    m_Toolbar->AddTool(wxWindowId::OPEN_DB, "Open database", wxArtProvider::GetBitmap("wxART_FOLDER_OPEN"));
    m_Toolbar->SetToolLongHelp(wxWindowId::OPEN_DB, "Open database");

    m_Toolbar->AddTool(wxWindowId::NEW_DB, "New database", wxArtProvider::GetBitmap("wxART_NEW_DIR"));
    m_Toolbar->SetToolLongHelp(wxWindowId::NEW_DB, "Create a new database");

    m_Toolbar->AddTool(wxWindowId::DROP_DB, "Delete database", wxArtProvider::GetBitmap("wxART_DELETE"));
    m_Toolbar->SetToolLongHelp(wxWindowId::DROP_DB, "Delete database");

    m_Toolbar->Realize();

    // create viewport
    auto* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    m_TablesPanel = new TableSelectionPanel(this, wxWindowId::OPEN_TABLE, wxSize(200, 0));
    mainSizer->Add(m_TablesPanel, 0, wxEXPAND);

    m_RecordsPanel = new RecordsViewPanel(this, wxID_ANY);
    mainSizer->Add(m_RecordsPanel, 1, wxEXPAND);

    SetSizer(mainSizer);
}

void Window::OnExit(wxCommandEvent& event) { Close(true); }

void Window::OnOpenDB(wxCommandEvent& event)
{
    auto* dialog = new DBSelectionDialog(this, wxID_ANY, "Open database");
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        m_Db->OpenDb(dialog->GetDbName());
        UpdateUI();
    }
    dialog->Destroy();
}

void Window::OnCreateDB(wxCommandEvent& event)
{
    auto* dialog = new DBCreationDialog(this, wxID_ANY);
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        m_Db->CreateDb(dialog->GetDbName());
        UpdateUI();
    }
    dialog->Destroy();
}

void Window::OnDropDB(wxCommandEvent& event)
{
    auto* dialog = new DBSelectionDialog(this, wxID_ANY, "Delete database");
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        const std::string& dbName = dialog->GetDbName();
        const auto& message = wxString::Format("Are you sure you want to delete: %s ?", dbName.c_str());
        auto* confirmDialog = new wxMessageDialog(dialog, message, "Delete database", wxYES | wxNO);
        int confirmId = confirmDialog->ShowModal();
        if (confirmId == wxID_YES) m_Db->DropDb(dbName);
        confirmDialog->Destroy();
        if (!m_Db->IsDbOpen()) wxFrame::SetStatusText("No database is opened", 0);
    }

    dialog->Destroy();
}

void Window::UpdateUI()
{
    if (!m_IsTableToolAdded)
    {
        // add tool for table
        m_Toolbar->AddTool(wxWindowId::NEW_TABLE, "Create Table", wxArtProvider::GetBitmap("wxART_NEW"));
        m_Toolbar->SetToolLongHelp(wxWindowId::NEW_TABLE, "Create a new table");
        m_Toolbar->AddTool(wxWindowId::DROP_TABLE, "Delete Table", wxArtProvider::GetBitmap("wxART_CROSS_MARK"));
        m_Toolbar->SetToolLongHelp(wxWindowId::DROP_TABLE, "Delete the active table");
        m_Toolbar->Realize();

        wxMenu* menu = GetMenuBar()->GetMenu(0);
        menu->InsertSeparator(3);
        menu->Insert(4, wxWindowId::NEW_TABLE, "&Create table \tCtrl-Shift-N", "Create a new table");
        menu->Insert(5, wxWindowId::DROP_TABLE, "&Delete table \tCtrl-Shift-X", "Delete the active table");
        m_IsTableToolAdded = true;
    }

    SetStatusText(wxString::Format("Opened database: %s", m_Db->GetDbName().c_str()), 0);
    wxFrame::SetStatusText("", 1);
    const std::vector<std::string>& tables = m_Db->GetTableNames();
    wxString str;
    for (const auto& it: tables) str += it + " ";
    if (m_TablesPanel) m_TablesPanel->ShowTablesList(tables);
}

void Window::OnCreateTable(wxCommandEvent& event)
{
    auto* dialog = new TableCreationDialog(this, wxID_ANY);
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        m_Db->CreateTable(dialog->GetTableName(), dialog->GetColumns());
        UpdateUI();
        UpdateTableViewUI(m_Db->GetTable()->GetName());
    }
    dialog->Destroy();
}

void Window::OnOpenTable(wxCommandEvent& event)
{
    if (m_TablesPanel == nullptr) return;
    const std::string& tableName = m_TablesPanel->GetTableName();
    m_Db->OpenTable(tableName);
    UpdateTableViewUI(tableName);
}

void Window::UpdateTableViewUI(const std::string& tableName)
{
    SetStatusText(wxString::Format("Active table: %s", tableName.c_str()), 1);
    if (m_RecordsPanel == nullptr) return;
    m_RecordsPanel->ShowRecords(m_Db->GetTable()->GetData(), m_Db->GetTable()->GetColumns());
}

void Window::OnDropTable(wxCommandEvent& event)
{
    const std::string& tableName = m_Db->GetTable()->GetName();
    const auto& message = wxString::Format("Are you sure you want to delete: %s ?", tableName.c_str());
    auto* confirmDialog = new wxMessageDialog(this, message, "Delete table", wxYES | wxNO);
    int confirmId = confirmDialog->ShowModal();
    if (confirmId == wxID_YES) m_Db->DropTable();
    confirmDialog->Destroy();
    UpdateUI();
}
