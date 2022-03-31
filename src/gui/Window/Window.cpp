#include "Window.h"

Window::Window(const wxString& name, const wxSize& size)
        : wxFrame(nullptr, wxID_ANY, name, wxDefaultPosition, size, wxDEFAULT_FRAME_STYLE, name)
{
    // create the file menu
    auto* fileMenu = new wxMenu();
    // & underlines the first character so it can be accessible via the ALT+F shortcut
    fileMenu->Append(wxID_EXIT, "&Quit \tCtrl-Q");

    // create the help menu
    auto* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT);

    // create the menu bar
    auto* menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");
    wxFrame::SetMenuBar(menuBar);

    // bind the events to the methods
    Bind(wxEVT_MENU, &Window::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Window::OnExit, this, wxID_EXIT);
}

void Window::OnExit(wxCommandEvent& event) { Close(true); }

void Window::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("A simple SQL Like Database management system", "About", wxOK | wxICON_INFORMATION);
}
