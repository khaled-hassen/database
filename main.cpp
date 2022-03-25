#include <iostream>
#include "CSVFile.h"

int main()
{
    try
    {
        CSVFile csvFile;
        // csvFile.createNewFile("database.csv");
        // csvFile.createColumns({ "name", "address", "phone number" });
        // csvFile.addRow({ "khaled", "o utca", "123456" });
        // csvFile.addRow({ "hamza", "budapest", "987654" });
        // csvFile.save();
        csvFile.read("database.csv");
        csvFile.close();
    } catch (const std::exception& err)
    {
        std::cout << err.what() << std::endl;
        return 1;
    }

    return 0;
}
