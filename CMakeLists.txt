cmake_minimum_required(VERSION 4.0)
project(Bank)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 20)

add_executable(Bank main.cpp
        include/account.h
        src/transaction.cpp
        include/transaction.h
        include/stock/server.h
        src/stock/server.cpp
        include/messageQueue.h
        include/stock/visitor.h
        src/stock/visitor.cpp
        include/utils.h
        src/utils.cpp
)
