//
// Created by ali on 11/13/25.
//

#include <iostream>

#include "stock/server.h"

int main() {
    auto& server = stock::server::getInstance();
    std::thread stockUpdaterThread([&]() {
        server.startUpdateWorker();
    });
    std::thread stockOrderThread([&]() {
        server.startOrderWorker();
    });
    stockUpdaterThread.join();
    stockOrderThread.join();

    int input;
    std::cin >> input;

    switch (input) {




        default: std::cout << "Unknown input" << std::endl;
    }
}
