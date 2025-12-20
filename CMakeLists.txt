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
        include/stock/Server.h
        src/stock/Server.cpp
        include/stock/MsgQueue.h
        include/stock/variant.h
        include/utils.h
        src/utils.cpp
        src/stock/MsgQueue.cpp
        include/Bank.h
        src/Bank.cpp
        main.cpp
        src/logger.cpp
        src/stock/StockAccount.cpp
        include/stock/StockAccount.h
)
