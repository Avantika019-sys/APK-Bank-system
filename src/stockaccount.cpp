#include "stockaccount.h"

#include "transaction.h"

#include "stock/server.h"
#include <iostream>
#include <stdexcept>
StockAccount::StockAccount(std::string name, std::string id)
    : Account(name, id) {
  auto handler = std::bind(&StockAccount::onStockUpdate, this,
                           std::placeholders::_1, std::placeholders::_2);
  stock::server::getInstance().sig.connect(handler);
  monitorStocks = false;
}

void StockAccount::buyStock(int amountOfStocks, std::string stockName) {
  auto &serv = stock::server::getInstance();

  stock::info i(stockName);
  auto stockInfoFut = i.prom.get_future();
  stock::variant infoVariant(std::move(i));
  serv.msgQueue.push(std::move(infoVariant));

  stockInfoFut.wait();
  int stockPrice = stockInfoFut.get();
  int totalPrice = stockPrice * amountOfStocks;
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

  // stockTx stockTx(amountOfStocks, stockName);
  stock::order o;
  auto orderFut = o.prom.get_future();
  stock::variant orderVariant(std::move(o));
  serv.msgQueue.push(std::move(orderVariant));

  orderFut.wait();
  std::lock_guard<std::mutex> lock(mtx_);
  if (orderFut.get()) {
    Tx tx(totalPrice, TxType::stockPurchase);
    txs_.push_back(tx);
    if (ownedStocks_.contains(stockName)) {
      ownedStocks_[stockName] += amountOfStocks;
    } else {
      ownedStocks_[stockName] = amountOfStocks;
    }
    return;
  }
  throw std::invalid_argument(
      "server failed to process purchase please try later");
}
void StockAccount::setMonitorStocks(bool monitor) {
  std::lock_guard<std::mutex> lock(mtx_);
  monitorStocks = monitor;
}
void StockAccount::onStockUpdate(std::string stockName, int updatedPrice) {
  std::lock_guard<std::mutex> lock(mtx_);
  if (ownedStocks_.contains(stockName) && monitorStocks) {
    std::cout << "The value of a stock you own has updated!" << std::endl;
    std::cout << "Stock name:" << stockName << std::endl;
    std::cout << "New price per stock:" << updatedPrice << std::endl;
  }
}
StockAccount::StockAccount(StockAccount &&other)
    : Account(std::move(other)), ownedStocks_(std::move(other.ownedStocks_)) {}
StockAccount &StockAccount::operator=(StockAccount &&other) {
  if (this != &other) {
    Account::operator=(std::move(other));
    ownedStocks_ = std::move(other.ownedStocks_);
  }
  return *this;
}

void StockAccount::sellStock(int amount, std::string name) {
  std::lock_guard<std::mutex> lock(mtx_);
  auto it = ownedStocks_.find("name");
  if (it == ownedStocks_.end()) {
    throw std::invalid_argument("you do not own this stock");
  }
  if (it->second < amount) {
    throw std::invalid_argument(
        "you do not own enough of this stock too sell that amount");
  }
  stock::info i(name);
  auto stockInfoFut = i.prom.get_future();
  stock::variant infoVariant(std::move(i));
  auto &serv = stock::server::getInstance();
  serv.msgQueue.push(std::move(infoVariant));

  stockInfoFut.wait();
  int stockPrice = stockInfoFut.get();
  int totalSellValue = stockPrice * amount;

  std::cout << "Confirm you would like to sell stock, you will get: "
            << totalSellValue << "\nPlease confirm purchase by pressing y: ";
  char inp;
  std::cin >> inp;
  if (inp != 'y') {
    std::cout << "stock sell cancelled";
    return;
  }
  stock::order o;
  auto orderFut = o.prom.get_future();
  stock::variant orderVariant(std::move(o));
  serv.msgQueue.push(std::move(orderVariant));

  orderFut.wait();
  Tx tx(amount, TxType::stockSell);
  txs_.push_back(tx);
}
