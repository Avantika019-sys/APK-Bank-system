#include "../../include/stock/Server.h"
#include "stock/messages/Info.h"
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
  std::uniform_real_distribution<double> distrib(-0.005, 0.005);

  while (run) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::lock_guard<std::mutex> lock(mtx);
    for (auto &[stockName, stock] : stocks_) {
      double percentChange = distrib(gen);
      double newPrice = stock.prices.back() * (1 + percentChange);
      stock.prices.push_back(newPrice);
      stock.signal(stockName, newPrice);
    }
  }
}

void Server::startMessageProccesor() {
  struct visitor {
    Server &serv;
    void operator()(messages::OrderRequest &o) {
      messages::OrderResponse resp(true);
      o.prom.set_value(resp);
    }
    void operator()(messages::InfoRequest &i) {
      double trend = serv.calculateStockTrend(i.stockName);
      int currentPrice = serv.stocks_[i.stockName].prices.back();
      i.prom.set_value(messages::InfoResponse(currentPrice, trend));
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
