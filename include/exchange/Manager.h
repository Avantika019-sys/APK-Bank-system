#include "Account.h"
#include "Server.h"
#include "currency/DKK.h"
#include "exchange/message/Info.h"
#include "message.hpp"
#include "trait.hpp"
#include "tx.hpp"
#include "util/Spinner.h"
#include <boost/signals2/connection.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef EXCHANGE_ASSETMANAGER_H
#define EXCHANGE_ASSETMANAGER_H
using namespace std::chrono_literals;
namespace exchange {
template <typename T> struct ownedAsset {
  trait::Precision<T>::PrecisionT qty;
  boost::signals2::connection conn;
  std::optional<currency::DKK> stopLossRule;
};
template <typename T> class Manager {
public:
  Manager(Server<T> &serv, boost::shared_ptr<Account> acc,
          std::string managerId)
      : serv_(serv), acc(acc), managerId_(managerId) {}
  message::InfoResponse getInfo(std::vector<std::string> symbol) const {
    message::InfoRequest i{{symbol}};
    auto infoF = i.prom.get_future();
    serv_.pushMsg(message::Message<T>(std::move(i)));

    while (infoF.wait_for(100ms) != std::future_status::ready) {
      util::spin("Getting info");
    }
    std::cout << std::endl;
    return infoF.get();
  }

  void purchaseAsset(std::string symbol, currency::DKK amountInDKK) {
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Started purchase of " << symbol << std::endl;
    auto infoResp = getInfo({symbol});

    auto assetInfo = infoResp.assetInfos[0];
    typedef typename trait::Precision<T>::PrecisionT PrecisionT;
    PrecisionT qtyPurchaseable =
        (PrecisionT)amountInDKK.value() / assetInfo.currentPrice.value();
    auto balance = acc->getBalance();
    std::cout << "Quantity you can get for " << amountInDKK.value()
              << " DKK: " << qtyPurchaseable << "\n"
              << "Current trend: " << assetInfo.trend << "%"
              << "\nPlease confirm purchase by pressing y: ";
    char inp;
    std::cin >> inp;
    if (inp != 'y') {
      std::cout << "stock purchase cancelled";
      return;
    }

    message::OrderRequest o{symbol, managerId_, amountInDKK,
                            message::OrderType::BUY};
    auto orderF = o.prom.get_future();
    serv_.pushMsg(message::Message<T>(std::move(o)));

    while (orderF.wait_for(100ms) != std::future_status::ready) {
      util::spin("waiting for server to process purchase order");
    }
    std::cout << std::endl;

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
        assetInfo.currentPrice,
        currency::DKK(-amountInDKK.value()),
    });
    std::lock_guard<std::mutex> lock(mtx_);
    portfolio_[symbol].qty += qtyPurchaseable;
  }
  void sellAsset(std::string symbol, currency::DKK amountInDKK) {
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Started sale of " << symbol << std::endl;
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = portfolio_.find(symbol);
    if (it == portfolio_.end()) {
      throw std::invalid_argument("you do not own this stock");
    }
    auto infoResp = getInfo({symbol});
    auto assetInfo = infoResp.assetInfos[0];
    typedef typename trait::Precision<T>::PrecisionT PrecisionT;
    PrecisionT qtyToSell =
        static_cast<PrecisionT>(amountInDKK.value()) /
        static_cast<PrecisionT>(assetInfo.currentPrice.value());
    if (it->second.qty < qtyToSell) {
      throw std::invalid_argument(
          "you do not own enough of this stock too sell" +
          amountInDKK.toString() + " worth of " + symbol);
    }

    std::cout << "Confirm you would like to sell " << qtyToSell << " " << symbol
              << " aka. " << amountInDKK << " worth of " << symbol
              << "\nPlease confirm sale by pressing y: ";
    char inp;
    std::cin >> inp;
    if (inp != 'y') {
      std::cout << "stock sell cancelled";
      return;
    }
    message::OrderRequest o{symbol, managerId_, amountInDKK,
                            message::OrderType::SELL};
    auto orderFut = o.prom.get_future();
    serv_.pushMsg(message::Message<T>(std::move(o)));

    while (orderFut.wait_for(100ms) != std::future_status::ready) {
      util::spin("waiting for server to process sale order");
    }
    std::cout << std::endl;
    orderFut.wait();
    if (orderFut.get().isSucceded) {
      int precision = trait::Precision<T>::PrecisionFormat();
      currency::DKK total(qtyToSell * assetInfo.currentPrice.value());
      acc->addTransaction(tx::Sale{symbol, trait::Print<T>::Header(),
                                   std::to_string(qtyToSell),
                                   assetInfo.currentPrice, total});
    }
    portfolio_[symbol].qty -= qtyToSell;
  }
  void printPortfolioStats() const {
    std::cout << "-----------------------------------" << std::endl;
    std::cout << trait::Print<T>::Header() + " PORTFOLIO stats:\n" << std::endl;
    std::vector<std::string> AssetSymbols;
    for (auto &[symbol, _] : portfolio_) {
      AssetSymbols.push_back(symbol);
    }
    auto resp = getInfo(AssetSymbols);
    double trendSum = 0;
    double valueSum = 0;
    for (const auto &[symbol, price, trend] : resp.assetInfos) {
      auto qty = portfolio_.at(symbol).qty;
      // clang-format off
      std::cout << 
        "Symbol: " << symbol << "\n" <<
        "Quantity owned: " << qty << "\n" <<
        "Price per unit: " << price.toString() << "\n" << 
        "Total value: " << price.value()*qty << "\n"<<
        "Trend: " << trend<< "%\n\n"
      ;
      // clang-format on
      trendSum += trend;
      valueSum += price.value() * qty;
    }
    std::cout << "Portfolio Trend: " << trendSum / resp.assetInfos.size() << "%"
              << std::endl;
    std::cout << "Portfolio Value: " << valueSum << std::endl;
  }

  void addStopLossRule(std::string symbol, currency::DKK limit) {
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
  void onAssetUpdate(std::string symbol, currency::DKK updatedPrice) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = portfolio_.find(symbol);
    if (it == portfolio_.end()) {
      return;
    }
    if (!it->second.stopLossRule.has_value()) {
      return;
    }
    auto limit = it->second.stopLossRule.value();
    if (updatedPrice.value() <= limit.value()) {
      message::OrderRequest o(symbol, managerId_, it->second.qty,
                              message::OrderType::SELL);
      auto orderFut = o.prom.get_future();
      serv_.pushMsg(message::Message<T>(std::move(o)));

      orderFut.wait();
      if (!orderFut.get().isSucceded) {
        throw std::invalid_argument("server failed to process sale");
      }
      currency::DKK total(it->second.qty * updatedPrice.value());
      int precision = trait::Precision<T>::PrecisionFormat();
      acc->addTransaction(
          tx::Sale{symbol, trait::Print<T>::Header(),
                   std::format("{:.{}f}", it->second.qty, precision),
                   updatedPrice, total});
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
