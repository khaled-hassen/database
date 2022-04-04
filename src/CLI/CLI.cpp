#include "CLI.h"
#include <iostream>
#include <vector>
#include <sstream>

CLI::CLI() : m_Exit(false) { std::cout << "Type help to display available commands" << std::endl; }

void CLI::showHelp()
{
    std::cout << "\tHelp: Commands\n"
              << "\t- open db <db-name>: Open database\n"
              << "\t- create db <db-name>: Create a new database \n"
              << "\t- drop db: delete the opened database\n"
              << "\t- close db: Close the opened database\n"
              << "\t- ShowRecords db: display all the table names in the database\n"
              << "\t- open table <table-name>: Open a table\n"
              << "\t- create table <table-name>: Create a new table\n"
              << "\t- drop table: delete the opened table\n"
              << "\t- search table <condition>: search table records based on the condition\n"
              << "\t- save table: save the changes in the opened table\n"
              << "\t- close table: close the opened table\n"
              << "\t- ShowRecords table: display all the records in the table\n"
              << "\t- insert record <column-1> <column-2> ... <column-n>: insert a new record in the opened table\n"
              << "\t- update record <record-id> <column-name> <new-value>...: update record\n"
              << "\t- drop record <record-id>: delete record"
              << std::endl;
}

std::vector<std::string> CLI::parseCommand(const std::string& _command)
{
    // these two commands don't have a target, so they're treated separately
    if (_command == "exit" || _command == "help") return std::vector<std::string>({ _command });

    std::stringstream stream(_command);
    std::vector<std::string> commandParts;
    std::string part;
    while (std::getline(stream, part, ' ')) if (!part.empty()) commandParts.push_back(part);
    if (commandParts.size() < 2) throw std::exception("Invalid command");
    return commandParts;
}

void CLI::listen()
{
    std::cout << ">";
    std::getline(std::cin, m_Command);
}

void CLI::execute()
{
    if (m_Command.empty()) return;
    std::vector<std::string> parsedCommand = parseCommand(m_Command);
    std::string action = parsedCommand[0];
    if (action == "help")
    {
        showHelp();
        return;
    }

    if (action == "exit")
    {
        m_Exit = true;
        return;
    }

    std::string target = parsedCommand[1];
    if (target == "db")
    {
        if (action == "Open") std::cout << "opening db" << std::endl;
        else if (action == "Create") std::cout << "creating db" << std::endl;
        else if (action == "drop") std::cout << "dropping" << std::endl;
        else if (action == "Close") std::cout << "closing db" << std::endl;
        else if (action == "ShowRecords") std::cout << "showing db" << std::endl;
        else throw std::exception("Invalid command");
    }
    else if (target == "table")
    {
        if (action == "Open") std::cout << "opening table" << std::endl;
        else if (action == "Create") std::cout << "creating table" << std::endl;
        else if (action == "drop") std::cout << "dropping table" << std::endl;
        else if (action == "save") std::cout << "saving table" << std::endl;
        else if (action == "Close") std::cout << "closing table" << std::endl;
        else if (action == "search") std::cout << "searching table" << std::endl;
        else if (action == "ShowRecords") std::cout << "showing table" << std::endl;
        else throw std::exception("Invalid command");
    }
    else if (target == "record")
    {
        if (action == "insert") std::cout << "inserting record" << std::endl;
        else if (action == "update") std::cout << "updating record" << std::endl;
        else if (action == "drop") std::cout << "dropping record" << std::endl;
        else throw std::exception("Invalid command");
    }
    else throw std::exception("Invalid command");
}
