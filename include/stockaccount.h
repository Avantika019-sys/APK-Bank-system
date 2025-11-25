#include "account.h"
#include "stock/server.h"

class StockAccount : public Account {
public:
  StockAccount() {
    auto handler = std::bind(
      &StockAccount::onStockUpdate,
      this,
      std::placeholders::_1,
      std::placeholders::_2);
    stock::server::getInstance().sig.connect(handler);
  }

  bool buyStock(int amountOfStocks, std::string stockName) {
    auto &serv = stock::server::getInstance();

    stock::info i(stockName);
    auto stockInfoFut = i.prom.get_future();
    stock::variant infoVariant(std::move(i));
    serv.msgQueue.push(std::move(infoVariant));

    stockInfoFut.wait();
    int stockPrice = stockInfoFut.get();
    int totalPrice = stockPrice * amountOfStocks;
    if (getCurrentBalance() < totalPrice) {
      char logMsg[50] = "attempt to buy stock failed because of low balance";
      fputs(logMsg, fptrLogs_);
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
      char logMsg[50] = "successfully bought stocks";
      fputs(logMsg, fptrLogs_);
      return true;
    }
    return false;
  }

  void sellStock(int amount, std::string name) {
  }

  void onStockUpdate(std::string stockName, int updatedPrice) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (ownedStocks_.contains(stockName)) {
      ownedStocks_[stockName] = updatedPrice;
    }
  }

private:
  std::vector<stockTx> stockTxs_;
  std::map<std::string, int> ownedStocks_;
  std::mutex mtx_;
};
