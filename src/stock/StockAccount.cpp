#include "stock/StockAccount.h"

#include "../../include/stock/messages/Info.h"
#include "logger.h"
#include "stock/MessageQueue.h"
#include "stock/Server.h"
#include <iostream>
#include <stdexcept>
namespace bank::stock {
StockAccount::StockAccount(std::string name, std::string id)
    : Account(name, id) {}

void StockAccount::buyStock(std::string stockName, int qty) {
  auto &serv = Server::getInstance();

  messages::InfoRequest i(stockName);
  auto infoF = i.prom.get_future();
  serv.pushMsg(Message(std::move(i)));

  infoF.wait();
  auto infoResp = infoF.get();
  int totalPrice = infoResp.latestPrice * qty;
  if (getBalance() < totalPrice) {
    throw std::invalid_argument("you dont have enough money to buy stock");
  }
  std::cout << "Total price: " << totalPrice
            << "\nPlease confirm purchase by pressing y: ";
  char inp;
  std::cin >> inp;
  if (inp != 'y') {
    std::cout << "stock purchase cancelled";
    return;
  }

  messages::Order o(stockName, qty, messages::OrderType::BUY);
  auto orderF = o.prom.get_future();
  serv.pushMsg(Message(std::move(o)));

  orderF.wait();
  bool success = orderF.get();
  if (!success) {
    throw std::invalid_argument(
        "server failed to process purchase please try later");
  }
  std::lock_guard<std::mutex> lock(mtx_);
  txs_.emplace_back(stockPurchaseDetails{stockName, qty, infoResp.latestPrice});
  if (portfolio_.contains(stockName)) {
    portfolio_[stockName].NoOfStocksOwned += qty;
  } else {
    portfolio_[stockName].NoOfStocksOwned = qty;
  }
}

void StockAccount::onStockUpdate(std::string stockName, int updatedPrice) {
  auto &serv = Server::getInstance();
  std::lock_guard<std::mutex> lock(mtx_);
  auto it = portfolio_.find(stockName);
  if (it == portfolio_.end()) {
    return;
  }
  if (!it->second.stopLossRule.has_value()) {
    return;
  }
  if (updatedPrice <= it->second.stopLossRule.value()) {
    messages::Order o(stockName, it->second.NoOfStocksOwned,
                      messages::OrderType::SELL);
    auto orderFut = o.prom.get_future();
    serv.pushMsg(Message(std::move(o)));

    orderFut.wait();
    if (!orderFut.get()) {
      throw std::invalid_argument(
          "server failed to process sale please try later");
    }
    Tx tx(stockSellDetails{stockName, it->second.NoOfStocksOwned, updatedPrice},
          nullptr);
    txs_.push_back(tx);
    logger_.log("automatically sold stock because of stock loss limit",
                level::INFO, "transaction", tx);
  }
}
StockAccount::StockAccount(StockAccount &&other) noexcept
    : Account(std::move(other)), portfolio_(std::move(other.portfolio_)) {}
StockAccount &StockAccount::operator=(StockAccount &&other) noexcept {
  if (this != &other) {
    Account::operator=(std::move(other));
    portfolio_ = std::move(other.portfolio_);
  }
  return *this;
}

void StockAccount::sellStock(std::string stockName, int qty) {
  auto &serv = Server::getInstance();

  std::lock_guard<std::mutex> lock(mtx_);
  auto it = portfolio_.find(stockName);
  if (it == portfolio_.end()) {
    throw std::invalid_argument("you do not own this stock");
  }
  if (it->second.NoOfStocksOwned < qty) {
    throw std::invalid_argument(
        "you do not own enough of this stock too sell that amount");
  }
  messages::InfoRequest i(stockName);
  auto f = i.prom.get_future();
  serv.pushMsg(Message(std::move(i)));

  f.wait();
  auto infoResp = f.get();
  int totalSellValue = infoResp.latestPrice * qty;

  std::cout << "Confirm you would like to sell stock, you will get: "
            << totalSellValue << "\nPlease confirm purchase by pressing y: ";
  char inp;
  std::cin >> inp;
  if (inp != 'y') {
    std::cout << "stock sell cancelled";
    return;
  }
  messages::Order o(stockName, qty, messages::OrderType::SELL);
  auto orderFut = o.prom.get_future();
  serv.pushMsg(Message(std::move(o)));

  orderFut.wait();
  if (orderFut.get()) {
    txs_.emplace_back(stockSellDetails{stockName, qty, infoResp.latestPrice});
  }
}
void StockAccount::printPortfolio() {
  int sum = 0;
  for (auto &[stockName, ownedStock] : portfolio_) {
    messages::InfoRequest i(stockName);
    auto f = i.prom.get_future();
    Server::getInstance().pushMsg(Message(std::move(i)));
    f.wait();
    auto infoResp = f.get();
    int totalValue = infoResp.latestPrice * ownedStock.NoOfStocksOwned;
    sum += totalValue;
    std::cout << "Stock name: " << stockName
              << " Amount of stock owned: " << ownedStock.NoOfStocksOwned
              << " Price per stock: " << infoResp.latestPrice
              << " Total value: " << totalValue;
  }
  std::cout << "Total value of portfolio: " << sum << std::endl;
}
void StockAccount::addStopLossRule(std::string name, int limit) {
  auto handler = std::bind(&StockAccount::onStockUpdate, this,
                           std::placeholders::_1, std::placeholders::_2);
  portfolio_[name].conn =
      Server::getInstance().getSignal(name).connect(handler);
}
void StockAccount::removeStopLossRule(std::string name, int limit) {
  portfolio_[name].conn.disconnect();
}
} // namespace bank::stock
