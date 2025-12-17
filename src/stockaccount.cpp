#include "stockaccount.h"

#include "transaction.h"

#include "stock/server.h"
StockAccount::StockAccount(std::string name, std::string id)
    : Account(name, id) {
  auto handler = std::bind(&StockAccount::onStockUpdate, this,
                           std::placeholders::_1, std::placeholders::_2);
  stock::server::getInstance().sig.connect(handler);
  monitorStocks = false;
}

bool StockAccount::buyStock(int amountOfStocks, std::string stockName) {
  auto &serv = stock::server::getInstance();

  stock::info i(stockName);
  auto stockInfoFut = i.prom.get_future();
  stock::variant infoVariant(std::move(i));
  serv.msgQueue.push(std::move(infoVariant));

  stockInfoFut.wait();
  int stockPrice = stockInfoFut.get();
  int totalPrice = stockPrice * amountOfStocks;
  if (getBalance() < totalPrice) {
    return false;
  }

  stockTx stockTx(amountOfStocks, stockName);
  stock::order o(stockTx);
  auto orderFut = o.prom.get_future();
  stock::variant orderVariant(std::move(o));
  serv.msgQueue.push(std::move(orderVariant));

  orderFut.wait();
  std::lock_guard<std::mutex> lock(mtx_);
  if (orderFut.get()) {
    moneyTx moneyTx(totalPrice, moneyTxType::stockPurchase);
    moneyTxs_.push_back(moneyTx);
    if (ownedStocks_.contains(stockName)) {
      ownedStocks_[stockName] += amountOfStocks;
    } else {
      ownedStocks_[stockName] = amountOfStocks;
    }
    return true;
  }
  return false;
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
    : Account(std::move(other)), stockTxs_(std::move(other.stockTxs_)),
      ownedStocks_(std::move(other.ownedStocks_)) {}
StockAccount &StockAccount::operator=(StockAccount &&other) {
  if (this != &other) {
    Account::operator=(std::move(other));
    stockTxs_ = std::move(other.stockTxs_);
    ownedStocks_ = std::move(other.ownedStocks_);
  }
  return *this;
}
