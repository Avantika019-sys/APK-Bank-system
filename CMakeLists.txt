cmake_minimum_required(VERSION 3.28)
project(Bank)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories("/usr/include/boost/")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Bank main.cpp
        include/account.h
        src/account.cpp
        src/Tx.cpp
        include/Tx.h
        include/stock/server.h
        src/stock/server.cpp
        include/stock/messageQueue.h
        include/stock/variant.h
        src/stock/visitor.cpp
        include/utils.h
        src/utils.cpp
        src/stock/msgQueue.cpp
        include/Bank.h
        src/bank.cpp
        main.cpp
        src/logger.cpp
        src/stock/StockAccount.cpp
        include/stock/StockAccount.h
)
