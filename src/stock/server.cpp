//
// Created by ali on 11/13/25.
//

#include "../../include/stock/server.h"
#include <random>
#include <bits/this_thread_sleep.h>

namespace stock {

server::server() : msgQueue(10) {
    std::vector<std::pair<std::string, double> > stocks = {
        {"AAPL", 189.45},
        {"MSFT", 332.64},
        {"GOOG", 130.17},
        {"TSLA", 240.01},
        {"NVDA", 455.72}
    };
}

void server::startUpdateWorker() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, stocks.size() - 1);

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        int random_index = distrib(gen);
        auto& stockToUpdate = stocks[random_index];
        stockToUpdate.second += 20;
        sig(stockToUpdate.first,stockToUpdate.second);
    }
}

void server::startOrderWorker() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::variant<order,stateReq> variant;
        msgQueue.pop(variant);
        std::visit(this->orderVisitor,variant);
    }
}
}
