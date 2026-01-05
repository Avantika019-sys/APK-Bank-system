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
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef EXCHANGE_MANAGER_H
#define EXCHANGE_MANAGER_H
using namespace std::placeholders;
using namespace std::chrono_literals;
namespace exchange {
template <typename T> struct ownedAsset {
  trait::Precision<T>::PrecisionT qty;
  boost::signals2::connection conn;
  std::optional<currency::DKK> stopLossRule;
};
template <typename T> class Manager {
public:
  Manager(boost::shared_ptr<Server<T>> serv, boost::shared_ptr<Account> acc,
          std::string managerId)
      : serv_(serv), acc(acc), managerId_(managerId) {}
  message::InfoResponse getInfo(std::vector<std::string> symbol) const {
    message::InfoRequest i{{symbol}};
    auto infoF = i.prom.get_future();
    serv_->pushMsg(message::Message<T>(std::move(i)));

    while (infoF.wait_for(100ms) != std::future_status::ready) {
      util::spin("Getting info");
    }
    std::cout << std::endl;
    return infoF.get();
  }

  void purchaseAsset(std::string symbol, currency::DKK amountInDKK) {
    std::cout << "-----------------------------------" << std::endl;
    std::string header = trait::Print<T>::Header();
    std::cout << "Started purchase of " << header << ": " << symbol
              << std::endl;
    auto infoResp = getInfo({symbol});

    auto assetInfo = infoResp.assetInfos[0];
    typedef typename trait::Precision<T>::PrecisionT PrecisionT;
    PrecisionT qtyPurchaseable =
        (PrecisionT)amountInDKK.value() / assetInfo.currentPrice.value();
    auto balance = acc->getBalance();
    std::cout << "Quantity you can get for " << amountInDKK.value() << " DKK: "
              << std::setprecision(
                     std::numeric_limits<PrecisionT>::max_digits10)
              << qtyPurchaseable << "\n"
              << std::setprecision(2) << "Current trend: " << assetInfo.trend
              << "%"
              << "\nPlease confirm purchase by pressing y: ";
    char inp;
    std::cin >> inp;
    if (inp != 'y') {
      std::cout << "Purchase cancelled" << std::endl;
      return;
    }

    message::OrderRequest o{symbol, managerId_, amountInDKK,
                            message::OrderType::BUY};
    auto orderF = o.prom.get_future();
    serv_->pushMsg(message::Message<T>(std::move(o)));

    while (orderF.wait_for(100ms) != std::future_status::ready) {
      util::spin("waiting for server to process purchase order");
    }
    std::cout << std::endl;

    auto resp = orderF.get();
    if (!resp.isSucceded) {
      throw std::invalid_argument(
          "server failed to process purchase please try later");
    }
    acc->addTransaction(tx::Purchase{
        symbol,
        trait::Print<T>::Header(),
        std::format("{}", qtyPurchaseable),
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
      throw std::invalid_argument("you do not own this asset");
    }
    auto infoResp = getInfo({symbol});
    auto assetInfo = infoResp.assetInfos[0];
    typedef typename trait::Precision<T>::PrecisionT PrecisionT;
    PrecisionT qtyToSell =
        static_cast<PrecisionT>(amountInDKK.value()) /
        static_cast<PrecisionT>(assetInfo.currentPrice.value());
    if (it->second.qty < qtyToSell) {
      throw std::invalid_argument(
          "you do not own enough of this asset too sell" +
          amountInDKK.toString() + " worth of " + symbol);
    }

    std::cout << "Confirm you would like to sell " << qtyToSell << " " << symbol
              << " aka. " << amountInDKK << " worth of " << symbol
              << "\nPlease confirm sale by pressing y: ";
    char inp;
    std::cin >> inp;
    if (inp != 'y') {
      std::cout << "sale cancelled";
      return;
    }
    message::OrderRequest o{symbol, managerId_, amountInDKK,
                            message::OrderType::SELL};
    auto orderFut = o.prom.get_future();
    serv_->pushMsg(message::Message<T>(std::move(o)));

    while (orderFut.wait_for(100ms) != std::future_status::ready) {
      util::spin("waiting for server to process sale order");
    }
    std::cout << std::endl;
    orderFut.wait();
    if (orderFut.get().isSucceded) {
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
    typedef typename trait::Precision<T>::PrecisionT PrecisionT;
    for (const auto &[symbol, price, trend] : resp.assetInfos) {
      auto qty = portfolio_.at(symbol).qty;
      std::cout << "Symbol: " << symbol << "\n"
                << "Quantity owned: "
                << std::setprecision(
                       std::numeric_limits<PrecisionT>::max_digits10)
                << qty << "\n"
                << std::setprecision(2)
                << "Price per unit: " << price.toString() << "\n"
                << "Total value: " << price.value() * qty << " DKK" << "\n"
                << "Trend: " << trend << "%\n\n";
      trendSum += trend;
      valueSum += price.value() * qty;
    }
    std::cout << "Portfolio Trend: " << trendSum / resp.assetInfos.size() << "%"
              << std::endl;
    std::cout << "Portfolio Value: " << valueSum << " DKK" << std::endl;
  }

  void addStopLossRule(std::string symbol, currency::DKK limit) {
    if (!portfolio_.contains(symbol)) {
      throw std::invalid_argument("U dont own this asset");
    };
    auto handler = std::bind(&Manager<T>::onAssetUpdate, this, symbol, _1);
    auto conn = serv_->subscribeToPriceUpdates(symbol, handler);
    portfolio_[symbol].stopLossRule.emplace(limit);
    portfolio_[symbol].conn = conn;
  }
  void removeStopLossRule(std::string name) {
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
      message::OrderRequest o(
          {symbol, managerId_, it->second.qty, message::OrderType::SELL});
      auto orderFut = o.prom.get_future();
      serv_->pushMsg(message::Message<T>(std::move(o)));
      currency::DKK total(it->second.qty * updatedPrice.value());
      acc->addTransaction(tx::Sale{symbol, trait::Print<T>::Header(),
                                   std::format("{}", it->second.qty),
                                   updatedPrice, total});
      portfolio_.erase(it);
    }
  }

  std::map<std::string, ownedAsset<T>> portfolio_;
  std::mutex mtx_;
  boost::shared_ptr<Server<T>> serv_;
  boost::shared_ptr<Account> acc;
  std::string managerId_;
};
} // namespace exchange
#endif // EXCHANGE_MANAGER_H
