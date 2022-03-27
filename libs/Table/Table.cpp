#include "Table.h"
#include "../Utils/ID/Id.h"
#include <iostream>
#include <string>

Table::Table()
{
    std::cout << "Creating table. Available data types: string, int, double \n";

    int number = 0;
    std::string input;
    while (true)
    {
        std::cout << "Enter the number of columns: ";
        std::getline(std::cin, input);
        try
        {
            number = std::stoi(input);
        } catch (...)
        {
            std::cout << "Invalid number\n";
            continue;
        }
        if (number > 0) break;
        std::cout << "Invalid number\n";
    }

    columns.reserve(number + 1);
    columns.insert({"id", ID().ToString()});
    int i = 1;
    while (i <= number)
    {
        std::cout << "Column" << i << " name: ";
        std::string name;
        std::getline(std::cin, name);
        std::string type;
        while (true)
        {
            std::cout << "Column" << i << " data type: ";
            std::getline(std::cin, type);
            if (type == "string" || type == "int" || type == "double")
            {
                ++i;
                break;
            }
            std::cout << "Invalid data type\n";
        }
        columns.insert({ name, type });
    }
    std::cout << std::endl;
}

void Table::Insert()
{

}
