#pragma once

#include <string>
#include <vector>

class CLI
{
private:
    // string is initialized by its constructor
    std::string m_Command;
    bool m_Exit;

// helper private methods
private:
    // display the help message
    static void showHelp();

    // parse the user _command
    [[nodiscard]] static std::vector<std::string> parseCommand(const std::string& _command);

public:
    CLI();

    // return if the cli is still closed or not
    // [[nodiscard]] will tell the compiler that the return value shouldn't be ignored (>= c++ 17)
    [[nodiscard]] bool isClosed() const { return m_Exit; }

    // listen for commands
    void listen();

    // execute a command
    void execute();
};
