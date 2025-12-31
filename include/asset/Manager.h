#include "MessageQueue.h"
#include "Server.h"
#include "asset/traits/Print.h"
#include "bank/Account.h"
#include "messages/Info.h"
#include "messages/Order.h"
#include "messages/PortfolioTrend.h"
#include "util/Logger.h"
#include "util/Spinner.h"
#include <boost/signals2/connection.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <iostream>
#include <map>
#include <stdexcept>
#include <unordered_set>

#ifndef BANK_ASSETMANAGER_H
#define BANK_ASSETMANAGER_H
using namespace std::chrono_literals;
namespace asset {
template <typename T> struct ownedAsset {
  traits::Precision<T>::PrecisionT qty;
  std::optional<double> stopLossRule;
  boost::signals2::connection conn;
};
template <typename T> class Manager {
public:
  Manager(Server<T> *serv, boost::shared_ptr<bank::Account> acc,
          util::Logger *logger)
      : serv(serv), acc(acc), logger(logger) {}
  asset::messages::InfoResponse<T> getInfo(std::string symbol) {
    messages::InfoRequest<T> i{symbol};
    auto infoF = i.prom.get_future();
    serv->pushMsg(Message<T>(std::move(i)));

    while (infoF.wait_for(10ms) != std::future_status::ready) {
      util::spin("Getting the current info for " + symbol);
    }
    return infoF.get();
  }

  void purchaseAsset(std::string symbol, double amountInDKK) {
    std::cout << "---------------------------" << std::endl;
    std::cout << "Started purchase of " << symbol << std::endl;
    auto infoResp = getInfo(symbol);

    typedef typename traits::Precision<T>::PrecisionT PrecisionT;
    PrecisionT qtyPurchaseable = amountInDKK / infoResp.currentPrice;
    auto balance = acc->getBalance();
    std::cout << "Quantity you can get for " << amountInDKK
              << "DKK: " << qtyPurchaseable
              << "\nPlease confirm purchase by pressing y: ";
    char inp;
    std::cin >> inp;
    if (inp != 'y') {
      std::cout << "stock purchase cancelled";
      return;
    }

    messages::OrderRequest<T> o{symbol, amountInDKK, messages::OrderType::BUY};
    auto orderF = o.prom.get_future();
    serv->pushMsg(Message<T>(std::move(o)));

    while (orderF.wait_for(10ms) != std::future_status::ready) {
      util::spin("waiting for server to process purchase order");
    }

    auto resp = orderF.get();
    if (!resp.isSucceded) {
      throw std::invalid_argument(
          "server failed to process purchase please try later");
    }
    int precision = traits::Precision<T>::PrecisionFormat();
    acc->addTransaction(tx::asset::Purchase{
        symbol,
        traits::Print<T>::Header(),
        std::format("{:.{}f}", qtyPurchaseable, precision),
        infoResp.currentPrice,
        amountInDKK,
    });
    std::lock_guard<std::mutex> lock(mtx_);
    if (portfolio_.contains(symbol)) {
      portfolio_[symbol].qty += qtyPurchaseable;
    } else {
      portfolio_[symbol].qty = qtyPurchaseable;
    }
  }
  void sellAsset(std::string symbol, double amountInDKK) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = portfolio_.find(symbol);
    if (it == portfolio_.end()) {
      throw std::invalid_argument("you do not own this stock");
    }
    if (it->second.qty < amountInDKK) {
      throw std::invalid_argument(
          "you do not own enough of this stock too sell that amount");
    }
    auto infoResp = getInfo(symbol);
    auto qtyToSell = amountInDKK / infoResp.currentPrice;

    std::cout << "Confirm you would like to sell " << qtyToSell << " " << symbol
              << "\nPlease confirm sale by pressing y: ";
    char inp;
    std::cin >> inp;
    if (inp != 'y') {
      std::cout << "stock sell cancelled";
      return;
    }
    messages::OrderRequest<T> o{symbol, amountInDKK, messages::OrderType::SELL};
    auto orderFut = o.prom.get_future();
    serv->pushMsg(Message<T>(std::move(o)));

    while (orderFut.wait_for(10ms) != std::future_status::ready) {
      util::spin("waiting for server to process sale order");
    }
    orderFut.wait();
    if (orderFut.get().isSucceded) {
      int precision = traits::Precision<T>::PrecisionFormat();
      acc->addTransaction(
          ::tx::asset::Sale{symbol, traits::Print<T>::Header(),
                            std::format("{:.{}f}", amountInDKK, precision),
                            infoResp.currentPrice});
    }
  }
  void printPortfolioTrend() {
    std::unordered_set<T> ownedAssets;
    for (auto &[assetName, _] : portfolio_) {
      ownedAssets.insert(assetName);
    }
    messages::PortfolioTrendRequest<T> p{ownedAssets};
    auto f = p.prom.get_future();
    std::cout << "Portfolio Trend: " << f.get();
  }
  void printPortfolio() {
    double sum = 0;
    std::cout << "---------------------------" << std::endl;
    std::cout << traits::Print<T>::Header() + " PORTFOLIO:\n" << std::endl;
    for (auto &[symbol, ownedAsset] : portfolio_) {
      messages::InfoRequest<T> i{symbol};
      auto f = i.prom.get_future();
      serv->pushMsg(Message<T>(std::move(i)));
      f.wait();
      auto infoResp = f.get();
      double totalValue = infoResp.currentPrice * ownedAsset.qty;
      sum += totalValue;
      std::cout << "Symbol: " << symbol
                << "\nQuantity owned: " << ownedAsset.qty
                << "\nPrice per stock: " << infoResp.currentPrice
                << "\nTotal value: " << totalValue << std::endl
                << std::endl;
    }
    std::cout << "Total value of portfolio: " << sum << std::endl;
  }

  void addStopLossRule(std::string symbol, int limit) {
    if (!portfolio_.contains(symbol)) {
      throw std::invalid_argument("U dont own this asset");
    };
    auto handler = std::bind(&Manager<T>::onAssetUpdate, this,
                             std::placeholders::_1, std::placeholders::_2);
    portfolio_[symbol].conn = serv->getSignal(symbol).connect(handler);
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
    auto handler = std::bind(&Manager<T>::onAssetUpdate, this,
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
      auto handler = std::bind(&Manager<T>::onAssetUpdate, this,
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
  void onAssetUpdate(std::string symbol, double updatedPrice) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = portfolio_.find(symbol);
    if (it == portfolio_.end()) {
      return;
    }
    if (!it->second.stopLossRule.has_value()) {
      return;
    }
    if (updatedPrice <= it->second.stopLossRule.value()) {
      messages::OrderRequest<T> o(symbol, it->second.qty,
                                  messages::OrderType::SELL);
      auto orderFut = o.prom.get_future();
      serv->pushMsg(Message<T>(std::move(o)));

      orderFut.wait();
      if (!orderFut.get().isSucceded) {
        throw std::invalid_argument("server failed to process sale");
      }
      auto total = it->second.qty * updatedPrice;
      int precision = traits::Precision<T>::PrecisionFormat();
      acc->addTransaction(
          tx::asset::Sale{symbol, traits::Print<T>::Header(),
                          std::format("{:.{}f}", it->second.qty, precision),
                          updatedPrice, total});
      logger->log("automatically sold " + symbol +
                      " because of stop loss limit",
                  util::level::INFO,
                  util::field("stop loss value limit",
                              it->second.stopLossRule.value()));
    }
  }
  std::map<std::string, ownedAsset<T>> portfolio_;
  std::mutex mtx_;
  Server<T> *serv;
  boost::shared_ptr<bank::Account> acc;
  util::Logger *logger;
};
} // namespace asset
#endif // BANK_ASSETACCOUNT_H
