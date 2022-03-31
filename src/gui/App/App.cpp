#include "App.h"
#include "../Window/Window.h"

bool App::OnInit()
{
    auto* window = new Window("Database", wxSize(1000, 600));
    window->Show(true);
    return true;
}
