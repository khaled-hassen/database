cmake_minimum_required(VERSION 3.21)
project(database)

set(CMAKE_CXX_STANDARD 23)

add_executable(database main.cpp libs/CSVFile/CSVFile.cpp libs/CSVFile/CSVFile.h libs/CLI/CLI.cpp libs/CLI/CLI.h libs/Database/Database.cpp libs/Database/Database.h libs/Table/Table.cpp libs/Table/Table.h libs/Utils/Pointer/Pointer.h libs/Utils/ID/Id.h libs/Utils/String/StringUtils.cpp libs/Utils/String/StringUtils.h)
