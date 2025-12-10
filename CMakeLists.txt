cmake_minimum_required(VERSION 3.28)
project(Bank)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Bank main.cpp
        include/account.h
        src/account.cpp
        src/transaction.cpp
        include/transaction.h
        include/stock/server.h
        src/stock/server.cpp
        include/stock/messageQueue.h
        include/stock/visitor.h
        src/stock/visitor.cpp
        include/utils.h
        src/utils.cpp
        src/stock/messageQueue.cpp
        include/bank.h
        src/bank.cpp
        main.cpp
        src/logger.cpp
)
