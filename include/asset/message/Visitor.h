#include "asset/Calculator.h"
#include "asset/Server.h"
#include "asset/message/Queue.h"
#include "asset/message/types/Stop.h"
#include "asset/traits/Print.h"
#include "asset/traits/Server.h"
#include "asset/traits/Trend.h"
#include "asset/types/Crypto.h"
#include "asset/types/Stock.h"
#include "util/Logger.h"
#include <boost/signals2/signal.hpp>
#ifndef BANK_MESSAGEVISITOR_H
#define BANK_MESSAGEVISITOR_H
namespace asset::message {
template <typename T> struct MessageVisitor {
  Server<T> &serv;
  void operator()(types::OrderRequest<T> &o) {
    serv.logger_->log("Received order request", util::level::INFO,
                      util::field{"Quantity", o.qty},
                      util::field{traits::Print<T>::Header(), o.assetName},
                      util::field{"Type", o.getTypeStr()},
                      util::field{"manager id", o.managerId});
    types::OrderResponse resp(true);
    o.prom.set_value(resp);
    for (auto cb : serv.OrderEventCbs) {
      cb(o.assetName, o.qty, 100, 100, 102, true);
    }
  }
  void operator()(types::InfoRequest<T> &i) {
    serv.logger_->log("Received info request", util::level::INFO,
                      util::field(traits::Print<T>::Header(), i.assetName));
    auto &asset = serv.assets_.at(i.assetName);
    auto trend = calculateTrendForIndividualAsset<T>(asset);
    double price = serv.assets_.at(i.assetName).priceOverTime_.back();
    i.prom.set_value(types::InfoResponse<T>{price, trend});
  }
  void operator()(types::PortfolioTrendRequest<T> &p) {
    std::vector<T *> assets;
    for (auto assetName : p.ownedAssets) {
      assets.push_back(&serv.assets_.at(assetName));
    }
    typedef typename traits::Precision<T>::PrecisionT PrecisionT;
    double trend;
    if (p.ownedAssets.size() * traits::Trend<T>::LookBackPeriod() < 1000) {
      trend = CalculatePortfolioTrend(assets, sequential{});
    } else {
      trend = CalculatePortfolioTrend(assets, parallel{});
    }
    p.prom.set_value(trend);
  }
  void operator()(types::Stop &s) { serv.run_ = false; }
  void operator()(types::OrderEvent<T> &o) {
    serv.OrderEventCbs.push_back(o.cb);
  }
};
template <> struct MessageVisitor<types::Crypto> {
  Server<types::Crypto> &serv;
  void operator()(types::OrderRequest<types::Crypto> &o) {
    serv.logger_->log(
        "Received order request", util::level::INFO,
        util::field{"Quantity", o.qty},
        util::field{traits::Print<types::Crypto>::Header(), o.assetName},
        util::field{"Type", o.getTypeStr()});
    types::OrderResponse resp(true);
    o.prom.set_value(resp);
    for (auto cb : serv.OrderEventCbs) {
      cb(o.assetName, o.qty, 100, 100, 102, true);
    }
  }
  void operator()(types::InfoRequest<types::Crypto> &i) {
    serv.logger_->log(
        "Received info request", util::level::INFO,
        util::field(traits::Print<types::Crypto>::Header(), i.assetName));
    auto &asset = serv.assets_.at(i.assetName);
    auto trend = calculateTrendForIndividualAsset<types::Crypto>(asset);
    double price = asset.priceOverTime_.back();
    i.prom.set_value(InfoResponse<types::Crypto>{price, trend});
  }
  void operator()(types::PortfolioTrendRequest<types::Crypto> &p) {
    std::vector<types::Crypto *> assets;
    for (auto assetName : p.ownedAssets) {
      assets.push_back(&serv.assets_.at(assetName));
    }
    typedef typename traits::Precision<types::Crypto>::PrecisionT AccT;
    AccT trend;
    if (p.ownedAssets.size() * traits::Trend<types::Crypto>::LookBackPeriod() <
        1000) {
      trend = CalculatePortfolioTrend(assets, sequential{});
    } else {
      trend = CalculatePortfolioTrend(assets, parallel{});
    }
    p.prom.set_value(trend);
  }
  void operator()(types::Stop &s) { serv.run_ = false; }
  void operator()(types::crypto::MineEvent &m) {
    std::lock_guard<std::mutex> lock(serv.mtx_);
    auto &crypto = serv.assets_.at(m.cryptoName);
    crypto.totalCoinsOnMarket += m.qty;
    double priceAffect = crypto.priceOverTime_.back() * m.qty;
    crypto.priceOverTime_.push_back(crypto.priceOverTime_.back() - priceAffect);
  }
  void operator()(types::OrderEvent<types::Crypto> &o) {
    serv.OrderEventCbs.push_back(o.cb);
  }
};
} // namespace asset::message
#endif // BANK_MESSAGEVISITOR_H
