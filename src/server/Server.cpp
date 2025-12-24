#include "../../include/server/Server.h"
#include "Bank.h"
#include "asset/Calculator.h"
#include "stock/messages/Info.h"
#include <bits/this_thread_sleep.h>
#include <random>
#include <utility>
#include <variant>

namespace bank::server {
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

// void Server::startSimulatingStockPriceUpdates() {
//   std::random_device rd;
//   std::mt19937 gen(rd());
//   std::uniform_real_distribution<double> distrib(-0.005, 0.005);
//
//   while (run) {
//     std::this_thread::sleep_for(std::chrono::seconds(3));
//     std::lock_guard<std::mutex> lock(mtx);
//     for (auto &[stockName, stock] : stocks_) {
//       double percentChange = distrib(gen);
//       double newPrice = stock.prices.back() * (1 + percentChange);
//       stock.prices.push_back(newPrice);
//       stock.signal(stockName, newPrice);
//     }
//   }
// }
//
// void Server::startMessageProccesor() {
// struct visitor {
//   Server &serv;
//   void operator()(messages::OrderRequest &o) {
//     messages::OrderResponse resp(true);
//     o.prom.set_value(resp);
//   }
//   void operator()(messages::InfoRequest &i) {
//     double trend = serv.calculateTrendForStock(i.stockName);
//     int currentPrice = serv.stocks_[i.stockName].prices.back();
//     i.prom.set_value(messages::InfoResponse(currentPrice, trend));
//   }
//   void operator()(messages::PortfolioTrend &p) {
//     int totalDataPoints = 0;
//     for (auto stock : p.ownedStocks) {
//       totalDataPoints += serv.stocks_[stock].prices.size();
//     }
//     double trend = 0;
//     if (totalDataPoints > 1000) {
//       CalculatePortfolioTrend(, sequential)
//     } else {
//     }
//     p.prom.set_value(trend);
//   }
//   void operator()(messages::Stop &s) { serv.run = false; }
//   };
//   while (run) {
//     auto msg = msgQueue_.pop();
//     std::visit(visitor{*this}, msg);
//   }
// }

// void Server::pushMsg(Message &&msg) { msgQueue_.push(std::move(msg)); }
} // namespace bank::server
