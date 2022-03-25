cmake_minimum_required(VERSION 3.21)
project(database)

set(CMAKE_CXX_STANDARD 23)

add_executable(database main.cpp CSVFile.cpp CSVFile.h)
