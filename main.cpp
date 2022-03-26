#include <iostream>
#include "libs/CLI/CLI.h"
#include "libs/Utils/Pointer/Pointer.h"
#include "libs/Database/Database.h"

int main()
{
    // CLI cli;
    Pointer<Database> db(new Database());
    // while (true)
    // {
    try
    {
        // db->Create("users");
        db->Open("users");
        // db->Show();
        // db->Drop();
        // db->Drop("users");
        db->Close();
        // cli.listen();
        // cli.execute();
    }
    catch (const std::exception& error)
    { std::cout << error.what() << std::endl; }

    // if (cli.isClosed()) break;
    // }

    return 0;
}
