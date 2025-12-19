#include "../../include/stock/server.h"
#include "stock/msgQueue.h"
#include <bits/this_thread_sleep.h>
#include <random>
#include <variant>

namespace stock {
server::server() : msgQueue_(10) {
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
  struct visitor {
    server &serv;
    void operator()(order &o) { o.prom.set_value(true); }
    void operator()(info &i) { i.prom.set_value(10); }
    void operator()(stop &s) {}
  };
  while (run) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    auto msg = msgQueue_.pop();
    std::visit(visitor{*this}, msg);
  }
}

void server::pushMsg(variant &&msg) { msgQueue_.push(std::move(msg)); }
void server::stopWorkers() {
  run = false;
  msgQueue_.push(stop{});
}
} // namespace stock
