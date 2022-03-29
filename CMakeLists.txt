cmake_minimum_required(VERSION 3.21)
project(database)

set(CMAKE_CXX_STANDARD 23)

add_executable(database main.cpp src/CSVFile/CSVFile.cpp src/CSVFile/CSVFile.h src/CLI/CLI.cpp src/CLI/CLI.h src/Database/Database.cpp src/Database/Database.h src/Table/Table.cpp src/Table/Table.h src/Utils/Pointer/Pointer.h src/Utils/ID/Id.h src/Utils/String/StringUtils.cpp src/Utils/String/StringUtils.h src/Utils/types/types.h)
