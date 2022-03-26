#include <iostream>
#include "libs/CLI/CLI.h"

int main()
{
    CLI cli;
    while (true)
    {
        try
        {
            cli.listen();
            cli.execute();
        }
        catch (const std::exception& error)
        { std::cout << error.what() << std::endl; }

        if (cli.isClosed()) break;
    }

    return 0;
}
