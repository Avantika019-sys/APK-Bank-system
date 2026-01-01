#include "Account.h"
#include "Server.h"
#include "message.hpp"
#include "trait.hpp"
#include "tx.hpp"
#include "util/Spinner.h"
#include <boost/signals2/connection.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <iostream>
#include <map>
#include <stdexcept>
#include <unordered_set>

#ifndef EXCHANGE_ASSETMANAGER_H
#define EXCHANGE_ASSETMANAGER_H
using namespace std::chrono_literals;
namespace exchange {
template <typename T> struct ownedAsset {
  trait::Precision<T>::PrecisionT qty;
  boost::signals2::connection conn;
  std::optional<double> stopLossRule;
};
template <typename T> class Manager {
public:
  Manager(Server<T> &serv, boost::shared_ptr<Account> acc,
          std::string managerId)
      : serv_(serv), acc(acc), managerId_(managerId) {}
  message::InfoResponse<T> getInfo(std::string symbol) {
    message::InfoRequest<T> i{symbol};
    auto infoF = i.prom.get_future();
    serv_.pushMsg(message::Message<T>(std::move(i)));

    while (infoF.wait_for(10ms) != std::future_status::ready) {
      util::spin("Getting the current info for " + symbol);
    }
    return infoF.get();
  }

  void purchaseAsset(std::string symbol, double amountInDKK) {
    std::cout << "---------------------------" << std::endl;
    std::cout << "Started purchase of " << symbol << std::endl;
    auto infoResp = getInfo(symbol);

    typedef typename trait::Precision<T>::PrecisionT PrecisionT;
    PrecisionT qtyPurchaseable = amountInDKK / infoResp.currentPrice;
    auto balance = acc->getBalance();
    std::cout << "Quantity you can get for " << amountInDKK
              << " DKK: " << qtyPurchaseable
              << "\nPlease confirm purchase by pressing y: ";
    char inp;
    std::cin >> inp;
    if (inp != 'y') {
      std::cout << "stock purchase cancelled";
      return;
    }

    message::OrderRequest<T> o{symbol, managerId_, amountInDKK,
                               message::OrderType::BUY};
    auto orderF = o.prom.get_future();
    serv_.pushMsg(message::Message<T>(std::move(o)));

    while (orderF.wait_for(10ms) != std::future_status::ready) {
      util::spin("waiting for server to process purchase order");
    }

    auto resp = orderF.get();
    if (!resp.isSucceded) {
      throw std::invalid_argument(
          "server failed to process purchase please try later");
    }
    int precision = trait::Precision<T>::PrecisionFormat();
    acc->addTransaction(tx::Purchase{
        symbol,
        trait::Print<T>::Header(),
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
    message::OrderRequest<T> o{symbol, managerId_, amountInDKK,
                               message::OrderType::SELL};
    auto orderFut = o.prom.get_future();
    serv_.pushMsg(message::Message<T>(std::move(o)));

    while (orderFut.wait_for(10ms) != std::future_status::ready) {
      util::spin("waiting for server to process sale order");
    }
    orderFut.wait();
    if (orderFut.get().isSucceded) {
      int precision = trait::Precision<T>::PrecisionFormat();
      acc->addTransaction(
          tx::Sale{symbol, trait::Print<T>::Header(),
                   std::format("{:.{}f}", amountInDKK, precision),
                   infoResp.currentPrice});
    }
  }
  void printPortfolioTrend() {
    std::unordered_set<T> ownedAssets;
    for (auto &[assetName, _] : portfolio_) {
      ownedAssets.insert(assetName);
    }
    message::PortfolioTrendRequest<T> p{ownedAssets};
    serv_.pushMsg(p);
    auto f = p.prom.get_future();
    std::cout << "Portfolio Trend: " << f.get();
  }
  void printPortfolio() {
    double sum = 0;
    std::cout << "---------------------------" << std::endl;
    std::cout << trait::Print<T>::Header() + " PORTFOLIO:\n" << std::endl;
    for (auto &[symbol, ownedAsset] : portfolio_) {
      message::InfoRequest<T> i{symbol};
      auto f = i.prom.get_future();
      serv_.pushMsg(message::Message<T>(std::move(i)));
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
    auto conn = serv_.subscribeToPriceUpdates(symbol, handler);
    portfolio_[symbol].stopLossRule.emplace(limit);
    portfolio_[symbol].conn = conn;
  }
  void removeStopLossRule(std::string name, int limit) {
    portfolio_[name].conn.disconnect();
  }
  Manager(const Manager &) = delete;
  Manager &operator=(const Manager &) = delete;
  Manager(Manager &&other) = delete;
  Manager &operator=(Manager &&other) = delete;

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
    auto limit = it->second.stopLossRule.value();
    if (updatedPrice <= limit) {
      message::OrderRequest<T> o(symbol, managerId_, it->second.qty,
                                 message::OrderType::SELL);
      auto orderFut = o.prom.get_future();
      serv_.pushMsg(message::Message<T>(std::move(o)));

      orderFut.wait();
      if (!orderFut.get().isSucceded) {
        throw std::invalid_argument("server failed to process sale");
      }
      auto total = it->second.qty * updatedPrice;
      int precision = trait::Precision<T>::PrecisionFormat();
      acc->addTransaction(
          tx::Sale{symbol, trait::Print<T>::Header(),
                   std::format("{:.{}f}", it->second.qty, precision),
                   updatedPrice, total});
      // logger->log("automatically sold " + symbol +
      //                 " because of stop loss limit",
      //             util::level::INFO,
      //             util::field("stop loss value limit",
      //                         it->second.stopLossRule.value()));
    }
  }
  std::map<std::string, ownedAsset<T>> portfolio_;
  std::mutex mtx_;
  Server<T> &serv_;
  boost::shared_ptr<Account> acc;
  std::string managerId_;
};
template <typename T, typename... Args>
std::unique_ptr<Manager<T>> createManager(Args &&...args) {
  return std::make_unique<Manager<T>>(std::forward<Args>(args)...);
}
} // namespace exchange
#endif // EXCHANGE_ASSETACCOUNT_H
