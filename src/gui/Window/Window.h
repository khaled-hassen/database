#pragma once

#include "../wxwidgets.h"

class Window : public wxFrame
{
public:
    Window(const wxString& name, const wxSize& size);

private:
    // handle close event
    void OnExit(wxCommandEvent& event);

    // handle the about event
    void OnAbout(wxCommandEvent& event);
};


