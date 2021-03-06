#include "App.h"
#include "gui/Window/Window.h"
#include <crtdbg.h>

bool App::OnInit()
{
    // call the parent class OnInit method
    if (!wxApp::OnInit()) return false;
    // deleting the pointer in the destructor crashes the program. wxWidgets will handle memory management
    auto* window = new Window("Database", wxSize(1000, 600));
    window->SetMinSize(wxSize(800, 500));
    window->Show(true);
    return true;
}

bool App::OnExceptionInMainLoop()
{
    try
    { throw; }
    catch (std::exception& e)
    { wxMessageBox(e.what(), "Error", wxICON_ERROR); }
    return true;
}
