#include "./Account.h"
#include "server/Server.h"
#include "server/messages/Info.h"
#include "server/messages/Order.h"
#include "server/messages/PortfolioTrend.h"
#include "server/messages/Stop.h"
#include <boost/signals2/connection.hpp>
#include <map>

#ifndef BANK_ASSETACCOUNT_H
#define BANK_ASSETACCOUNT_H
namespace bank::account {
struct ownedAsset {
  int NoOfStocksOwned;
  std::optional<int> stopLossRule;
  boost::signals2::connection conn;
};
template <typename T> class AssetAccount : public Account {
public:
  AssetAccount(std::string name, std::string id) : Account(name, id) {}
  void buyStock(std::string name, int qty) {
    auto &serv = server::Server<T>::getInstance();

    server::messages::InfoRequest<T> i(name);
    auto infoF = i.prom.get_future();
    serv.pushMsg(bank::server::Message<T>(std::move(i)));

    infoF.wait();
    auto infoResp = infoF.get();
    int totalPrice = infoResp.currentPrice * qty;
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

    server::messages::OrderRequest<T> o(name, qty,
                                        server::messages::OrderType::BUY);
    auto orderF = o.prom.get_future();
    serv.pushMsg(bank::server::Message<T>(std::move(i)));

    orderF.wait();
    auto resp = orderF.get();
    if (!resp.isSucceded) {
      throw std::invalid_argument(
          "server failed to process purchase please try later");
    }
    std::lock_guard<std::mutex> lock(mtx_);
    txs_.emplace_back(
        details(stockPurchaseDetails{name, qty, infoResp.currentPrice}),
        &pool_);
    if (portfolio_.contains(name)) {
      portfolio_[name].NoOfStocksOwned += qty;
    } else {
      portfolio_[name].NoOfStocksOwned = qty;
    }
  }
  void sellStock(std::string name, int qty) {
    auto &serv = server::Server<T>::getInstance();

    std::lock_guard<std::mutex> lock(mtx_);
    auto it = portfolio_.find(name);
    if (it == portfolio_.end()) {
      throw std::invalid_argument("you do not own this stock");
    }
    if (it->second.NoOfStocksOwned < qty) {
      throw std::invalid_argument(
          "you do not own enough of this stock too sell that amount");
    }
    server::messages::InfoRequest<T> i(name);
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
    server::messages::OrderRequest<T> o(name, qty,
                                        server::messages::OrderType::SELL);
    auto orderFut = o.prom.get_future();
    serv.pushMsg(Message(std::move(o)));

    orderFut.wait();
    if (orderFut.get().isSucceded) {
      txs_.emplace_back(
          details(stockSellDetails{name, qty, infoResp.currentPrice}), &pool_);
    }
  }
  void printPortfolio() {
    int sum = 0;
    for (auto &[stockName, ownedStock] : portfolio_) {
      server::messages::InfoRequest<T> i(stockName);
      auto f = i.prom.get_future();
      server::Server<T>::getInstance().pushMsg(
          bank::server::Message<T>(std::move(i)));
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

    auto handler = std::bind(&AssetAccount<T>::onStockUpdate, this,
                             std::placeholders::_1, std::placeholders::_2);
    portfolio_[name].conn =
        server::Server<T>::getInstance().getSignal(name).connect(handler);
  }
  void removeStopLossRule(std::string name, int limit) {
    portfolio_[name].conn.disconnect();
  }
  AssetAccount(const AssetAccount &) = delete;
  AssetAccount &operator=(const AssetAccount &) = delete;

  AssetAccount(AssetAccount &&other) noexcept : Account(std::move(other)) {
    for (auto &[name, asset] : other.portfolio_) {
      if (asset.conn.connected()) {
        asset.conn.disconnect();
      }
    }
    portfolio_ = std::move(other.portfolio_);
    auto handler = std::bind(&AssetAccount<T>::onStockUpdate, this,
                             std::placeholders::_1, std::placeholders::_2);
    for (auto &[name, asset] : portfolio_) {
      if (asset.stopLossRule.has_value()) {
        asset.conn =
            server::Server<T>::getInstance().getSignal(name).connect(handler);
      }
    }
  }
  AssetAccount &operator=(AssetAccount &&other) noexcept {
    if (this != &other) {
      Account::operator=(std::move(other));
      for (auto &[name, asset] : other.portfolio_) {
        if (asset.conn.connected()) {
          asset.conn.disconnect();
        }
      }
      portfolio_ = std::move(other.portfolio_);
      auto handler = std::bind(&AssetAccount<T>::onStockUpdate, this,
                               std::placeholders::_1, std::placeholders::_2);
      for (auto &[name, asset] : portfolio_) {
        if (asset.stopLossRule.has_value()) {
          asset.conn =
              server::Server<T>::getInstance().getSignal(name).connect(handler);
        }
      }
    }
    return *this;
  }

private:
  void onStockUpdate(std::string stockName, int updatedPrice) {
    auto &serv = server::Server<T>::getInstance();
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = portfolio_.find(stockName);
    if (it == portfolio_.end()) {
      return;
    }
    if (!it->second.stopLossRule.has_value()) {
      return;
    }
    if (updatedPrice <= it->second.stopLossRule.value()) {
      server::messages::OrderRequest<T> o(stockName, it->second.NoOfStocksOwned,
                                          server::messages::OrderType::SELL);
      auto orderFut = o.prom.get_future();
      serv.pushMsg(bank::server::Message<T>(std::move(o)));

      orderFut.wait();
      if (!orderFut.get().isSucceded) {
        throw std::invalid_argument(
            "server failed to process sale please try later");
      }
      Tx tx(
          stockSellDetails{stockName, it->second.NoOfStocksOwned, updatedPrice},
          nullptr);
      txs_.push_back(tx);
      logger_.log("automatically sold stock because of stock loss limit",
                  level::INFO, "transaction", tx);
    }
  }
  std::map<std::string, ownedAsset> portfolio_;
  std::mutex mtx_;
};

//    template<typename... Args>
// void place_buy_order(Args &&...args) {
//   order_history_.emplace_back(std::forward<Args>(args)...);
// }
//
// template <typename... Args> void place_sell_order(Args &&...args) {
//   order_history_.emplace_back(std::forward<Args>(args)...);
} // namespace bank::account
// StockAccount::StockAccount(std::string name, std::string id)
//     : Account(name, id) {}
//
#endif // BANK_ASSETACCOUNT_H
