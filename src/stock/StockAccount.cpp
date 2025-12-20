#include "stock/StockAccount.h"

#include "logger.h"
#include "stock/Server.h"
#include "stock/variant.h"
#include <iostream>
#include <stdexcept>
namespace bank::stock {
StockAccount::StockAccount(std::string name, std::string id)
    : Account(name, id) {
  auto handler = std::bind(&StockAccount::onStockUpdate, this,
                           std::placeholders::_1, std::placeholders::_2);
  stock::Server::getInstance().sig.connect(handler);
}

void StockAccount::buyStock(std::string stockName, uint qty) {
  auto &serv = stock::Server::getInstance();

  stock::Info i(stockName);
  auto infoF = i.prom.get_future();
  serv.pushMsg(stock::variant(std::move(i)));

  infoF.wait();
  uint pricePerStock = infoF.get();
  uint totalPrice = pricePerStock * qty;
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

  stock::order o(stockName, qty, stock::orderType::BUY);
  auto orderF = o.prom.get_future();
  serv.pushMsg(stock::variant(std::move(o)));

  orderF.wait();
  bool success = orderF.get();
  if (!success) {
    throw std::invalid_argument(
        "server failed to process purchase please try later");
  }
  std::lock_guard<std::mutex> lock(mtx_);
  txs_.emplace_back(stockPurchaseDetails{stockName, qty, pricePerStock});
  if (portfolio_.contains(stockName)) {
    portfolio_[stockName].first += qty;
  } else {
    portfolio_[stockName].first = qty;
  }
}

void StockAccount::onStockUpdate(std::string stockName, uint updatedPrice) {
  auto &serv = stock::Server::getInstance();
  std::lock_guard<std::mutex> lock(mtx_);
  auto it = portfolio_.find(stockName);
  if (it == portfolio_.end()) {
    return;
  }
  if (!it->second.second.has_value()) {
    return;
  }
  if (updatedPrice <= it->second.second.value()) {
    stock::order o(stockName, it->second.first, stock::orderType::SELL);
    auto orderFut = o.prom.get_future();
    serv.pushMsg(stock::variant(std::move(o)));

    orderFut.wait();
    if (!orderFut.get()) {
      throw std::invalid_argument(
          "server failed to process sale please try later");
    }
    Tx tx(stockSellDetails{stockName, it->second.first, updatedPrice}, nullptr);
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

void StockAccount::sellStock(std::string stockName, uint qty) {
  auto &serv = stock::Server::getInstance();

  std::lock_guard<std::mutex> lock(mtx_);
  auto it = portfolio_.find(stockName);
  if (it == portfolio_.end()) {
    throw std::invalid_argument("you do not own this stock");
  }
  if (it->second.first < qty) {
    throw std::invalid_argument(
        "you do not own enough of this stock too sell that amount");
  }
  stock::Info i(stockName);
  auto f = i.prom.get_future();
  serv.pushMsg(stock::variant(std::move(i)));

  f.wait();
  uint stockPrice = f.get();
  uint totalSellValue = stockPrice * qty;

  std::cout << "Confirm you would like to sell stock, you will get: "
            << totalSellValue << "\nPlease confirm purchase by pressing y: ";
  char inp;
  std::cin >> inp;
  if (inp != 'y') {
    std::cout << "stock sell cancelled";
    return;
  }
  stock::order o(stockName, qty, stock::orderType::SELL);
  auto orderFut = o.prom.get_future();
  serv.pushMsg(stock::variant(std::move(o)));

  orderFut.wait();
  if (orderFut.get()) {
    txs_.emplace_back(stockSellDetails{stockName, qty, stockPrice});
  }
}
void StockAccount::printPortfolio() {
  uint sum = 0;
  for (auto stock : portfolio_) {
    Info i(stock.first);
    auto f = i.prom.get_future();
    stock::Server::getInstance().pushMsg(stock::variant(std::move(i)));
    f.wait();
    uint pricePerStock = f.get();
    uint totalValue = pricePerStock * stock.second.first;
    sum += totalValue;
    std::cout << "Stock name: " << stock.first
              << " Amount of stock owned: " << stock.second.first
              << " Price per stock: " << pricePerStock
              << " Total value: " << totalValue;
  }
  std::cout << "Total value of portfolio: " << sum << std::endl;
}
void StockAccount::addStopLossRule(std::string name, uint limit) {}
void StockAccount::removeStopLossRule(std::string name, uint limit) {}
} // namespace bank::stock
