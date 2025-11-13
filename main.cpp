
#include <iostream>

#include "stock/server.h"


int main() {
    auto& server = stock::server::getInstance();
    std::thread stockUpdaterThread([&]() {
        server.startUpdateStocksWorker();
    });
    std::thread stockOrderThread([&]() {
        server.startStockWorker();
    });

    int input;
    std::cin >> input;

    switch (input) {




        default: std::cout << "Unknown input" << std::endl;
    }
    stockUpdaterThread.join();
    stockOrderThread.join();
}
