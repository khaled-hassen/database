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
        // db->CreateDb("users");
        db->OpenDb("users");
        db->OpenTable("table1");
        // db->DropTable("table2");
        // db->CreateTable("table1");
        db->InsertRecord();
        db->SaveTable();
        db->CloseTable();
        // db->InsertRecord();
        // db->ShowTables();
        // db->DropDb();
        // db->DropDb("users");
        db->CloseDb();
        // cli.listen();
        // cli.execute();
    }
    catch (const std::exception& error)
    { std::cout << error.what() << std::endl; }

    // if (cli.isClosed()) break;
    // }

    return 0;
}
