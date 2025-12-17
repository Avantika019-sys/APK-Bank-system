#include "../../include/stock/server.h"
#include <bits/this_thread_sleep.h>
#include <random>
#include <variant>

namespace stock {
server::server() : msgQueue(10) {
  stocks = {
      {"AAPL", 189.45},
      {"MSFT", 332.64},
      {"GOOG", 130.17},
      {"TSLA", 240.01},
  };
}

server &server::getInstance() {
  static server instance;
  return instance;
}

void server::startUpdateStocksWorker() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> indexDistrib(0, 3);
  std::uniform_int_distribution<int> priceDistrib(1, 1000);

  while (run) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    int random_index = indexDistrib(gen);
    auto &stockToUpdate = stocks[random_index];
    stockToUpdate.second = priceDistrib(gen);
    sig(stockToUpdate.first, stockToUpdate.second);
  }
}

void server::startStockWorker() {
  while (run) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    auto msg = msgQueue.pop();
    std::visit(this->orderVisitor, msg);
  }
}

void server::stopWorkers() {
  std::unique_lock<std::mutex> lock(mtx);
  run = false;
}
} // namespace stock
