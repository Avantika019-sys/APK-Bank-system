#include "../../include/stock/Server.h"
#include "stock/variant.h"
#include <bits/this_thread_sleep.h>
#include <functional>
#include <random>
#include <utility>
#include <variant>

namespace bank::stock {
Server::Server() : msgQueue_(10) {
  stocks_ = {
      {"AAPL", 189.45},
      {"MSFT", 332.64},
      {"GOOG", 130.17},
      {"TSLA", 240.01},
  };
  stocks_["AAPL"].push_back(189)
}

Server &Server::getInstance() {
  static Server instance;
  return instance;
}

void Server::startUpdateStocksWorker() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib(1, 1000);

  while (run) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    for (auto &stock : stocks_) {
      uint newPrice = distrib(gen);
      stock.second.push_back(newPrice);
      sig(stock.first, newPrice);
    }
  }
}

void Server::startStockWorker() {
  struct visitor {
    Server &serv;
    void operator()(order &o) { o.prom.set_value(true); }
    void operator()(Info &i) {
      double trend = serv.calculateStockTrend(i.stockName);
      i.prom.set_value(std::make_pair(serv.stocks_[i.stockName].back(), trend));
    }
    void operator()(PortfolioTrend &p) {
      std::vector<std::future<double>> futures;
      for (auto stock : p.ownedStocks) {
        futures.push_back(std::async(std::launch::async, [this, stock]() {
          return serv.calculateStockTrend(stock);
        }));
      }
      double total = 0;
      for (auto &f : futures) {
        total += f.get();
      }
      p.prom.set_value(total / futures.size());
    }
    void operator()(stop &s) {}
  };
  while (run) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    auto msg = msgQueue_.pop();
    std::visit(visitor{*this}, msg);
  }
}

void Server::pushMsg(variant &&msg) { msgQueue_.push(std::move(msg)); }
void Server::stopWorkers() {
  run = false;
  msgQueue_.push(stop{});
}
double Server::calculateStockTrend(std::string stockName) {
  auto &vec = stocks_[stockName];
  uint sumX = 0;
  uint sumY = 0;
  uint sumXY = 0;
  uint sumX2 = 0;
  for (int i = 0; i < vec.size(); i++) {
    uint price = vec[i];
    sumX = i;
    sumY += price;
    sumXY += (i * price);
    sumX2 += (i * i);
  }
  double numerator = (vec.size() * sumXY) - (sumX * sumY);
  double denominator = (vec.size() * sumX2) - (sumX * sumX);
  double m = numerator / denominator;
  double b = (sumY - m * sumX) / vec.size();
  double totalChange = m * (vec.size() - 1);
  return (totalChange / b) * 100;
}
} // namespace bank::stock
