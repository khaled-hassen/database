cmake_minimum_required(VERSION 3.21)
project(database)

set(CMAKE_CXX_STANDARD 23)

add_executable(database main.cpp libs/CSVFile/CSVFile.cpp libs/CSVFile/CSVFile.h libs/CLI/CLI.cpp libs/CLI/CLI.h)
