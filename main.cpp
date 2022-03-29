#include <iostream>
#include "src/CLI/CLI.h"
#include "src/Utils/Pointer/Pointer.h"
#include "src/Database/Database.h"

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
        // db->DropTable("table2");
        // db->CreateTable("table2");
        // db->GetTable()->Save();
        // db->CloseTable();
        // db->OpenTable("table1");
        db->OpenTable("table2");
        db->GetTable()->ShowRecords();
        db->GetTable()->InsertRecord();
        db->GetTable()->DeleteRecord(2);
        db->GetTable()->Save();
        db->CloseTable();
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
