#include "Window.h"
#include <wx/artprov.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include "../IDS.h"

// statically bind events to functions
BEGIN_EVENT_TABLE(Window, wxFrame)
                EVT_MENU(wxID_EXIT, Window::OnExit)
                EVT_LIST_ITEM_ACTIVATED(wxID_VIEW_LIST, Window::OnListItemSelected)
END_EVENT_TABLE()

Window::Window(const wxString& name, const wxSize& size)
        : wxFrame(nullptr, wxID_ANY, name, wxDefaultPosition, size, wxDEFAULT_FRAME_STYLE, name)
{
    // create the file menu
    auto* fileMenu = new wxMenu();
    // & underlines the first character, so it can be accessible via the ALT+F shortcut
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
    toolbar->AddTool(IDS::NEW_DB, "New database", wxArtProvider::GetBitmap("wxART_NEW_DIR"));
    toolbar->SetToolLongHelp(IDS::NEW_DB, "Create a new database");
    toolbar->AddTool(wxID_NEW, "New Table", wxArtProvider::GetBitmap("wxART_NEW"));
    toolbar->SetToolLongHelp(wxID_NEW, "Create a new table");
    toolbar->Realize();

    // create the viewport layout
    auto* splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                          wxSP_BORDER | wxSP_LIVE_UPDATE);
    auto* leftPanel = new wxPanel(splitter, wxID_ANY, wxDefaultPosition);
    leftPanel->SetBackgroundColour(wxColor("#DFDFDE"));

    auto* databasesList = new wxListView(leftPanel, wxID_VIEW_LIST);
    databasesList->AppendColumn("Databases");
    const std::vector<std::string>& items = Database::ScanForDatabases();
    for (const auto& item: items) databasesList->InsertItem(0, item);

    auto* rightPanel = new wxPanel(splitter);
    rightPanel->SetBackgroundColour(wxColor("#F7F5F2"));

    splitter->SetMinimumPaneSize(200);
    splitter->SplitVertically(leftPanel, rightPanel);
}

void Window::OnExit(wxCommandEvent& event) { Close(true); }

void Window::OnListItemSelected(wxListEvent& event)
{
    wxMessageBox(event.GetText());
}
