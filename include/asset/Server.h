#include "MessageQueue.h"
#include "asset/Calculator.h"
#include "asset/messages/Stop.h"
#include "asset/traits/Print.h"
#include "asset/traits/Server.h"
#include "asset/traits/Trend.h"
#include "types/Crypto.h"
#include "types/Stock.h"
#include "util/Logger.h"
#include <boost/signals2/signal.hpp>
#include <memory>
#include <random>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H
namespace asset {
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
      : msgQueue_(traits::MessageQueue<T>::QueueCapacity()),
        logger(std::move(logger)) {

    simulatorThread =
        std::thread([this]() { startSimulatingAssetPriceUpdates(); });
    messageProccessorThread =
        std::thread([this]() { startMessageProccesor(); });
  }

  void addAsset(std::string &&symbol, T &&asset) {
    std::lock_guard<std::mutex> lock(mtx);
    assets_.try_emplace(std::move(symbol), std::move(asset));
  }
  types::UpdateSignal *getSignal(std::string assetSymbol) {
    return assets_.at(assetSymbol).sig_;
  }
  void startSimulatingAssetPriceUpdates() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distrib(-0.005, 0.005);
    static_assert(hasPriceOverTime<T>::value);
    while (run) {
      std::this_thread::sleep_for(
          std::chrono::seconds(traits::Server<T>::UpdateRate()));
      std::lock_guard<std::mutex> lock(mtx);
      for (auto &[symbol, asset] : assets_) {
        double percentChange = distrib(gen);
        double newPrice = asset.priceOverTime_.back() * (1 + percentChange);
        asset.priceOverTime_.push_back(newPrice);
        (*asset.sig_)(symbol, newPrice);
      }
    }
  }

  void startMessageProccesor() {
    while (run) {
      auto msg = msgQueue_.pop();
      if (msg.valueless_by_exception()) {
        throw std::invalid_argument("hejj");
      }
      std::visit(MessageVisitor<T>{*this}, msg);
    }
  }
  void pushMsg(Message<T> &&msg) { msgQueue_.push(std::move(msg)); }
  ~Server() {
    pushMsg(messages::Stop());
    if (simulatorThread.joinable())
      simulatorThread.join();
    if (messageProccessorThread.joinable())
      messageProccessorThread.join();
  }

private:
  friend struct MessageVisitor<T>;
  MessageQueue<T> msgQueue_;
  std::map<std::string, T> assets_;
  std::mutex mtx;
  std::atomic<bool> run{true};
  std::vector<std::function<void(
      std::string assetName, int qty, int totalNoOfAssetForSale,
      int totalNoOfAssetDemand, double price, bool isBuy)>>
      OrderEventCbs;
  std::unique_ptr<util::Logger> logger;
  std::thread simulatorThread;
  std::thread messageProccessorThread;
};
template <typename T> struct MessageVisitor {
  Server<T> &serv;
  void operator()(messages::OrderRequest<T> &o) {
    serv.logger->log("Received order request", util::level::INFO,
                     util::field{"Quantity", o.qty},
                     util::field{traits::Print<T>::Header(), o.assetName},
                     util::field{"Type", o.getTypeStr()});
    messages::OrderResponse resp(true);
    o.prom.set_value(resp);
    for (auto cb : serv.OrderEventCbs) {
      cb(o.assetName, o.qty, 100, 100, 102, true);
    }
  }
  void operator()(messages::InfoRequest<T> &i) {
    serv.logger->log("Received info request", util::level::INFO,
                     util::field(traits::Print<T>::Header(), i.assetName));
    auto &asset = serv.assets_.at(i.assetName);
    auto trend = calculateTrendForIndividualAsset<T>(asset);
    double price = serv.assets_.at(i.assetName).priceOverTime_.back();
    i.prom.set_value(messages::InfoResponse<T>{price, trend});
  }
  void operator()(messages::PortfolioTrendRequest<T> &p) {
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
  void operator()(messages::Stop &s) { serv.run = false; }
  void operator()(messages::OrderEvent<T> &o) {
    serv.OrderEventCbs.push_back(o.cb);
  }
};
template <> struct MessageVisitor<types::Crypto> {
  Server<types::Crypto> &serv;
  void operator()(messages::OrderRequest<types::Crypto> &o) {
    serv.logger->log(
        "Received order request", util::level::INFO,
        util::field{"Quantity", o.qty},
        util::field{traits::Print<types::Crypto>::Header(), o.assetName},
        util::field{"Type", o.getTypeStr()});
    messages::OrderResponse resp(true);
    o.prom.set_value(resp);
    for (auto cb : serv.OrderEventCbs) {
      cb(o.assetName, o.qty, 100, 100, 102, true);
    }
  }
  void operator()(messages::InfoRequest<types::Crypto> &i) {
    serv.logger->log(
        "Received info request", util::level::INFO,
        util::field(traits::Print<types::Crypto>::Header(), i.assetName));
    auto &asset = serv.assets_.at(i.assetName);
    auto trend = calculateTrendForIndividualAsset<types::Crypto>(asset);
    double price = asset.priceOverTime_.back();
    i.prom.set_value(messages::InfoResponse<types::Crypto>{price, trend});
  }
  void operator()(messages::PortfolioTrendRequest<types::Crypto> &p) {
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
  void operator()(messages::Stop &s) { serv.run = false; }
  void operator()(messages::crypto::MineEvent &m) {
    std::lock_guard<std::mutex> lock(serv.mtx);
    auto &crypto = serv.assets_.at(m.cryptoName);
    crypto.totalCoinsOnMarket += m.qty;
    double priceAffect = crypto.priceOverTime_.back() * m.qty;
    crypto.priceOverTime_.push_back(crypto.priceOverTime_.back() - priceAffect);
  }
  void operator()(messages::OrderEvent<types::Crypto> &o) {
    serv.OrderEventCbs.push_back(o.cb);
  }
};
} // namespace asset

#endif // BANK_STOCKSERVER_H
