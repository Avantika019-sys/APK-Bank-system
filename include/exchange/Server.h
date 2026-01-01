#include "Calculator.h"
#include "asset.hpp"
#include "message.hpp"
#include "trait.hpp"
#include "util/Logger.h"
#include <boost/signals2/connection.hpp>
#include <boost/signals2/signal.hpp>
#include <functional>
#include <memory>
#include <random>
#include <shared_mutex>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#ifndef EXCHANGE_STOCKSERVER_H
#define EXCHANGE_STOCKSERVER_H
namespace exchange {
template <typename T, typename = void>
struct hasPriceOverTime : std::false_type {};
template <typename T>
struct hasPriceOverTime<T,
                        std::void_t<decltype(std::declval<T>().priceOverTime_)>>
    : std::true_type {};
template <typename T> struct MessageVisitor;
template <typename T> class Server {
public:
  Server(std::unique_ptr<util::Logger> logger)
      : msgQueue_(trait::MessageQueue<T>::QueueCapacity()),
        logger_(std::move(logger)) {

    simulatorThread =
        std::thread([this]() { startSimulatingAssetPriceUpdates(); });
    messageProccessorThread =
        std::thread([this]() { startMessageProccesor(); });
  }

  void addAsset(std::string &&symbol, T &&asset) {
    std::lock_guard<std::shared_mutex> lock(mtx_);
    assets_.try_emplace(std::move(symbol), std::move(asset));
  }

  boost::signals2::connection subscribeToPriceUpdates(
      std::string assetSymbol,
      std::function<void(std::string assetName, double UpdatedPrice)> cb) {
    return assets_.at(assetSymbol).sig_->connect(cb);
  }
  void startSimulatingAssetPriceUpdates() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distrib(-0.005, 0.005);
    static_assert(hasPriceOverTime<T>::value);
    while (run_) {
      std::this_thread::sleep_for(
          std::chrono::seconds(trait::Server<T>::UpdateRate()));
      std::lock_guard<std::shared_mutex> lock(mtx_);
      for (auto &[symbol, asset] : assets_) {
        double percentChange = distrib(gen);
        double newPrice = asset.priceOverTime_.back() * (1 + percentChange);
        asset.priceOverTime_.push_back(newPrice);
        (*asset.sig_)(symbol, newPrice);
      }
    }
  }

  void startMessageProccesor() {
    while (run_) {
      auto msg = msgQueue_.pop();
      std::visit(MessageVisitor<T>{*this}, msg);
    }
  }
  void pushMsg(message::Message<T> &&msg) { msgQueue_.push(std::move(msg)); }
  ~Server() {
    pushMsg(message::Stop());
    if (simulatorThread.joinable())
      simulatorThread.join();
    if (messageProccessorThread.joinable())
      messageProccessorThread.join();
  }

private:
  friend struct MessageVisitor<T>;
  message::Queue<T> msgQueue_;
  std::map<std::string, T> assets_;
  std::shared_mutex mtx_;
  std::atomic<bool> run_{true};
  std::vector<std::function<void(
      std::string assetName, int qty, int totalNoOfAssetForSale,
      int totalNoOfAssetDemand, double price, bool isBuy)>>
      OrderEventCbs;
  std::unique_ptr<util::Logger> logger_;
  std::thread simulatorThread;
  std::thread messageProccessorThread;
};

template <typename T> struct MessageVisitor {
  Server<T> &serv;
  void operator()(message::OrderRequest<T> &o) {
    serv.logger_->log("Received order request", util::level::INFO,
                      util::field{"Quantity", o.qty},
                      util::field{trait::Print<T>::Header(), o.assetName},
                      util::field{"Type", o.getTypeStr()},
                      util::field{"manager id", o.managerId});
    message::OrderResponse resp(true);
    o.prom.set_value(resp);
    for (auto cb : serv.OrderEventCbs) {
      cb(o.assetName, o.qty, 100, 100, 102, true);
    }
  }
  void operator()(message::InfoRequest<T> &i) {
    serv.logger_->log("Received info request", util::level::INFO,
                      util::field(trait::Print<T>::Header(), i.assetName));
    auto &asset = serv.assets_.at(i.assetName);
    auto trend = calculateTrendForIndividualAsset<T>(asset);
    double price = serv.assets_.at(i.assetName).priceOverTime_.back();
    i.prom.set_value(message::InfoResponse<T>{price, trend});
  }
  void operator()(message::PortfolioTrendRequest<T> &p) {
    std::vector<T *> assets;
    for (auto assetName : p.ownedAssets) {
      assets.push_back(&serv.assets_.at(assetName));
    }
    typedef typename trait::Precision<T>::PrecisionT PrecisionT;
    double trend;
    if (p.ownedAssets.size() * trait::Trend<T>::LookBackPeriod() < 1000) {
      trend = CalculatePortfolioTrend(assets, sequential{});
    } else {
      trend = CalculatePortfolioTrend(assets, parallel{});
    }
    p.prom.set_value(trend);
  }
  void operator()(message::Stop &s) { serv.run_ = false; }
};

template <> struct MessageVisitor<asset::Crypto> {
  Server<asset::Crypto> &serv;
  void operator()(message::OrderRequest<asset::Crypto> &o) {
    serv.logger_->log(
        "Received order request", util::level::INFO,
        util::field{"Quantity", o.qty},
        util::field{trait::Print<asset::Crypto>::Header(), o.assetName},
        util::field{"Type", o.getTypeStr()});
    message::OrderResponse resp(true);
    o.prom.set_value(resp);
    for (auto cb : serv.OrderEventCbs) {
      cb(o.assetName, o.qty, 100, 100, 102, true);
    }
  }
  void operator()(message::InfoRequest<asset::Crypto> &i) {
    serv.logger_->log(
        "Received info request", util::level::INFO,
        util::field(trait::Print<asset::Crypto>::Header(), i.assetName));
    auto &asset = serv.assets_.at(i.assetName);
    auto trend = calculateTrendForIndividualAsset<asset::Crypto>(asset);
    double price = asset.priceOverTime_.back();
    i.prom.set_value(InfoResponse<asset::Crypto>{price, trend});
  }
  void operator()(message::PortfolioTrendRequest<asset::Crypto> &p) {
    std::vector<asset::Crypto *> assets;
    for (auto assetName : p.ownedAssets) {
      assets.push_back(&serv.assets_.at(assetName));
    }
    typedef typename trait::Precision<asset::Crypto>::PrecisionT AccT;
    AccT trend;
    if (p.ownedAssets.size() * trait::Trend<asset::Crypto>::LookBackPeriod() <
        1000) {
      trend = CalculatePortfolioTrend(assets, sequential{});
    } else {
      trend = CalculatePortfolioTrend(assets, parallel{});
    }
    p.prom.set_value(trend);
  }
  void operator()(message::Stop &s) { serv.run_ = false; }
  void operator()(message::MineEvent &m) {
    std::lock_guard<std::shared_mutex> lock(serv.mtx_);
    auto &crypto = serv.assets_.at(m.cryptoName);
    crypto.totalCoinsOnMarket += m.qty;
    double priceAffect = crypto.priceOverTime_.back() * m.qty;
    crypto.priceOverTime_.push_back(crypto.priceOverTime_.back() - priceAffect);
  }
};
} // namespace exchange

#endif // EXCHANGE_STOCKSERVER_H
