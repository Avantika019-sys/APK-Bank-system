cmake_minimum_required(VERSION 3.28)
project(Bank)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories("/usr/include/boost/")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "include/*.h"
    "main.cpp"
)
add_executable(Bank ${SOURCES})
