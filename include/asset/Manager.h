#include "MessageQueue.h"
#include "Server.h"
#include "bank/Account.h"
#include "bank/TxDetails.h"
#include "messages/Info.h"
#include "messages/Order.h"
#include "messages/PortfolioTrend.h"
#include "messages/Stop.h"
#include "util/Logger.h"
#include <boost/signals2/connection.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <iostream>
#include <map>

#ifndef BANK_ASSETMANAGER_H
#define BANK_ASSETMANAGER_H
namespace asset {
struct ownedAsset {
  int NoOfStocksOwned;
  std::optional<int> stopLossRule;
  boost::signals2::connection conn;
};
template <typename T> class Manager {
public:
  Manager(Server<T> *serv, boost::shared_ptr<bank::Account> acc,
          util::Logger *logger)
      : serv(serv), acc(acc), logger(logger) {}
  void buyAsset(std::string name, int qty) {
    messages::InfoRequest<T> i(name);
    auto infoF = i.prom.get_future();
    serv->pushMsg(Message<T>(std::move(i)));

    infoF.wait();
    auto infoResp = infoF.get();
    int totalPrice = infoResp.currentPrice * qty;
    if (acc->getBalance() < totalPrice) {
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

    messages::OrderRequest<T> o(name, qty, messages::OrderType::BUY);
    auto orderF = o.prom.get_future();
    serv->pushMsg(Message<T>(std::move(i)));

    orderF.wait();
    auto resp = orderF.get();
    if (!resp.isSucceded) {
      throw std::invalid_argument(
          "server failed to process purchase please try later");
    }
    std::lock_guard<std::mutex> lock(mtx_);
    acc->addTransaction(stockPurchaseDetails{name, qty, infoResp.currentPrice});
    if (portfolio_.contains(name)) {
      portfolio_[name].NoOfStocksOwned += qty;
    } else {
      portfolio_[name].NoOfStocksOwned = qty;
    }
  }
  void sellAsset(std::string name, int qty) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = portfolio_.find(name);
    if (it == portfolio_.end()) {
      throw std::invalid_argument("you do not own this stock");
    }
    if (it->second.NoOfStocksOwned < qty) {
      throw std::invalid_argument(
          "you do not own enough of this stock too sell that amount");
    }
    messages::InfoRequest<T> i(name);
    auto f = i.prom.get_future();
    serv.pushMsg(Message(std::move(i)));

    f.wait();
    auto infoResp = f.get();
    int totalSellValue = infoResp.currentPrice * qty;

    std::cout << "Confirm you would like to sell stock, you will get: "
              << totalSellValue << "\nPlease confirm purchase by pressing y: ";
    char inp;
    std::cin >> inp;
    if (inp != 'y') {
      std::cout << "stock sell cancelled";
      return;
    }
    messages::OrderRequest<T> o(name, qty, messages::OrderType::SELL);
    auto orderFut = o.prom.get_future();
    serv.pushMsg(Message(std::move(o)));

    orderFut.wait();
    if (orderFut.get().isSucceded) {
      acc->addTransaction(
          details(stockSellDetails{name, qty, infoResp.currentPrice}));
    }
  }
  void printPortfolio() {
    int sum = 0;
    for (auto &[stockName, ownedStock] : portfolio_) {
      messages::InfoRequest<T> i(stockName);
      auto f = i.prom.get_future();
      serv->pushMsg(Message<T>(std::move(i)));
      f.wait();
      auto infoResp = f.get();
      int totalValue = infoResp.currentPrice * ownedStock.NoOfStocksOwned;
      sum += totalValue;
      std::cout << "Stock name: " << stockName
                << " Amount of stock owned: " << ownedStock.NoOfStocksOwned
                << " Price per stock: " << infoResp.currentPrice
                << " Total value: " << totalValue;
    }
    std::cout << "Total value of portfolio: " << sum << std::endl;
  }

  void addStopLossRule(std::string name, int limit) {

    auto handler = std::bind(&Manager<T>::onStockUpdate, this,
                             std::placeholders::_1, std::placeholders::_2);
    portfolio_[name].conn = serv.getSignal(name).connect(handler);
  }
  void removeStopLossRule(std::string name, int limit) {
    portfolio_[name].conn.disconnect();
  }
  Manager(const Manager &) = delete;
  Manager &operator=(const Manager &) = delete;

  Manager(Manager &&other) noexcept {
    for (auto &[name, asset] : other.portfolio_) {
      if (asset.conn.connected()) {
        asset.conn.disconnect();
      }
    }
    portfolio_ = std::move(other.portfolio_);
    auto handler = std::bind(&Manager<T>::onStockUpdate, this,
                             std::placeholders::_1, std::placeholders::_2);
    for (auto &[name, asset] : portfolio_) {
      if (asset.stopLossRule.has_value()) {
        asset.conn = serv().getSignal(name).connect(handler);
      }
    }
  }
  Manager &operator=(Manager &&other) noexcept {
    if (this != &other) {
      for (auto &[name, asset] : other.portfolio_) {
        if (asset.conn.connected()) {
          asset.conn.disconnect();
        }
      }
      portfolio_ = std::move(other.portfolio_);
      auto handler = std::bind(&Manager<T>::onStockUpdate, this,
                               std::placeholders::_1, std::placeholders::_2);
      for (auto &[name, asset] : portfolio_) {
        if (asset.stopLossRule.has_value()) {
          asset.conn = serv().getSignal(name).connect(handler);
        }
      }
    }
    return *this;
  }

private:
  void onStockUpdate(std::string stockName, int updatedPrice) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = portfolio_.find(stockName);
    if (it == portfolio_.end()) {
      return;
    }
    if (!it->second.stopLossRule.has_value()) {
      return;
    }
    if (updatedPrice <= it->second.stopLossRule.value()) {
      messages::OrderRequest<T> o(stockName, it->second.NoOfStocksOwned,
                                  messages::OrderType::SELL);
      auto orderFut = o.prom.get_future();
      serv.pushMsg(Message<T>(std::move(o)));

      orderFut.wait();
      if (!orderFut.get().isSucceded) {
        throw std::invalid_argument("server failed to process sale");
      }
      acc->addTransaction(stockSellDetails{
          stockName, it->second.NoOfStocksOwned, updatedPrice});
      logger->log("automatically sold stock because of stock loss limit",
                  util::level::INFO,
                  util::field("stop loss value limit",
                              it->second.stopLossRule.value()));
    }
  }
  std::map<std::string, ownedAsset> portfolio_;
  std::mutex mtx_;
  Server<T> *serv;
  boost::shared_ptr<bank::Account> acc;
  util::Logger *logger;
};
} // namespace asset
#endif // BANK_ASSETACCOUNT_H
