#pragma once

#include "gui/wxwidgets.h"

class App : public wxApp
{
public:
    bool OnInit() override;

    bool OnExceptionInMainLoop() override;
};


