#include "../../include/stock/Server.h"
#include <bits/this_thread_sleep.h>
#include <random>
#include <utility>
#include <variant>

namespace bank::stock {
Server::Server() : msgQueue_(10) {
  stocks_["AAPL"].prices.push_back(189);
  stocks_["MSFT"].prices.push_back(129);
  stocks_["GOOG"].prices.push_back(889);
  stocks_["TSLA"].prices.push_back(589);
}

Server &Server::getInstance() {
  static Server instance;
  return instance;
}
StockUpdateSignal &Server::getSignal(std::string stockName) {
  return stocks_[stockName].signal;
}

void Server::startSimulatingStockPriceUpdates() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib(1, 1000);

  while (run) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    for (auto &[stockName, stock] : stocks_) {
      int newPrice = distrib(gen);
      stock.prices.push_back(newPrice);
      stock.signal(stockName, newPrice);
    }
  }
}

void Server::startMessageProccesor() {
  struct visitor {
    Server &serv;
    void operator()(messages::Order &o) { o.prom.set_value(true); }
    void operator()(messages::Info &i) {
      double trend = serv.calculateStockTrend(i.stockName);
      i.prom.set_value(
          std::make_pair(serv.stocks_[i.stockName].prices.back(), trend));
    }
    void operator()(messages::PortfolioTrend &p) {
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
    void operator()(messages::Stop &s) { serv.run = false; }
  };
  while (run) {
    auto msg = msgQueue_.pop();
    std::visit(visitor{*this}, msg);
  }
}

void Server::pushMsg(Message &&msg) { msgQueue_.push(std::move(msg)); }
double Server::calculateStockTrend(std::string stockName) {
  auto &vec = stocks_[stockName].prices;
  int sumX = 0;
  int sumY = 0;
  int sumXY = 0;
  int sumX2 = 0;
  for (int i = 0; i < vec.size(); i++) {
    int price = vec[i];
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
