#include <iostream>
#include "CSVFile.h"

int main()
{
    try
    {
        CSVFile csvFile("database.csv");
        csvFile.createNewFile();
        csvFile.createColumns({ "name", "address", "phone number" });
        csvFile.addRow({ "khaled", "o utca", "123456" });
        csvFile.save();
        csvFile.close();
    } catch (const std::exception& err)
    {
        std::cout << err.what() << std::endl;
        return 1;
    }

    return 0;
}
