#include "Window.h"
#include <wx/artprov.h>
#include "../ID.h"
#include "../Dialog/Database/DbSelectionDialog.h"
#include "../Dialog/Database/DbCreationDialog.h"

// statically bind events to functions
BEGIN_EVENT_TABLE(Window, wxFrame)
                EVT_MENU(wxID_EXIT, Window::OnExit)
                EVT_MENU(ID::OPEN_DB, Window::OnOpenDB)
                EVT_MENU(ID::NEW_DB, Window::OnCreateDB)
END_EVENT_TABLE()

Window::Window(const wxString& title, const wxSize& size)
        : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size, wxDEFAULT_FRAME_STYLE, wxFrameNameStr)
{
    // initialize db
    db = Pointer(new Database());

    // create the file menu
    auto* fileMenu = new wxMenu();
    // & underlines the first character, so it can be accessible via the ALT+F shortcut
    fileMenu->Append(ID::OPEN_DB, "&Open database \tCtrl-O", "Open database");
    fileMenu->Append(ID::NEW_DB, "&Create database \tCtrl-N", "Create a new database");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "&Quit \tCtrl-Q");

    // create the menu bar
    auto* menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    wxFrame::SetMenuBar(menuBar);

    // create a status bar
    wxFrame::CreateStatusBar();
    wxFrame::SetStatusText("Ready");

    // create the toolbar
    auto* toolbar = wxFrame::CreateToolBar();

    toolbar->AddTool(ID::OPEN_DB, "Open database", wxArtProvider::GetBitmap("wxART_FOLDER_OPEN"));
    toolbar->SetToolLongHelp(ID::OPEN_DB, "Open database");

    toolbar->AddTool(ID::NEW_DB, "New database", wxArtProvider::GetBitmap("wxART_NEW_DIR"));
    toolbar->SetToolLongHelp(ID::NEW_DB, "Create a new database");

    toolbar->AddTool(ID::OPEN_TABLE, "Open Table", wxArtProvider::GetBitmap("wxART_FILE_OPEN"));
    toolbar->SetToolLongHelp(ID::OPEN_TABLE, "Open table");

    toolbar->AddTool(ID::NEW_TABLE, "New Table", wxArtProvider::GetBitmap("wxART_NEW"));
    toolbar->SetToolLongHelp(ID::NEW_TABLE, "Create a new table");

    toolbar->Realize();
}

void Window::OnExit(wxCommandEvent& event) { Close(true); }

void Window::OnOpenDB(wxCommandEvent& event)
{
    auto* dialog = new DBSelectionDialog(this, wxID_ANY);
    int id = dialog->ShowModal();

    if (id == wxID_OK) db->OpenDb(dialog->GetDbName());
    dialog->Destroy();
}

void Window::OnCreateDB(wxCommandEvent& event)
{
    auto* dialog = new DBCreationDialog(this, wxID_ANY);
    int id = dialog->ShowModal();

    if (id == wxID_OK) db->CreateDb(dialog->GetDbName());
    dialog->Destroy();
}
