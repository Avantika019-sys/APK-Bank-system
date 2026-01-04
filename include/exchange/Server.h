#include "Calculator.h"
#include "asset.hpp"
#include "exchange/currency/DKK.h"
#include "exchange/message/Info.h"
#include "exchange/util/observability.hpp"
#include "message.hpp"
#include "trait.hpp"
#include <boost/signals2/connection.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/smart_ptr/make_shared_array.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <functional>
#include <random>
#include <thread>
#include <type_traits>
#include <utility>
#ifndef EXCHANGE_SERVER_H
#define EXCHANGE_SERVER_H
using namespace std::chrono_literals;
using namespace exchange::util::observability;
namespace exchange {

template <typename T, typename = void>
struct hasPriceOverTime : std::false_type {};
template <typename T>
struct hasPriceOverTime<
    T, std::void_t<decltype(std::declval<T>().unitPriceOverTime_)>>
    : std::true_type {};

template <typename T> struct MessageVisitor;

template <typename T> class Server {
public:
  Server(Logger logger, const MonitorResource *ms)
      : msgQueue_(trait::MessageQueue<T>::QueueCapacity()),
        logger_(std::move(logger)), ms_(ms) {
    simulatorThread =
        std::thread([this]() { startSimulatingAssetPriceUpdates(); });
    messageProccessorThread =
        std::thread([this]() { startMessageProccesor(); });
  }

  void addAsset(std::string &&symbol, T &&asset) {
    std::lock_guard<std::mutex> lock(mtx_);
    assets_.try_emplace(std::move(symbol), std::move(asset));
  }

  boost::signals2::connection
  subscribeToPriceUpdates(std::string assetSymbol,
                          std::function<void(currency::DKK UpdatedPrice)> cb) {
    return assets_.at(assetSymbol).sig_->connect(cb);
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
  void startSimulatingAssetPriceUpdates() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distrib(-0.005, 0.005);
    std::bernoulli_distribution d(0.1);
    static_assert(hasPriceOverTime<T>::value);
    while (run_) {
      std::this_thread::sleep_for(
          std::chrono::seconds(trait::Server<T>::UpdateRate()));
      std::lock_guard<std::mutex> lock(mtx_);
      for (auto &[symbol, asset] : assets_) {
        double percentChange = distrib(gen);
        currency::DKK newPrice{asset.unitPriceOverTime_.back().value() *
                               (1 + percentChange)};
        asset.unitPriceOverTime_.push_back(newPrice);
        (*asset.sig_)(newPrice);
      }
      if (d(gen)) {
        logger_.log(
            "system health", util::observability::level::INFO,
            util::observability::field("bytes", ms_->getbytesalloc()),
            util::observability::field{"queue-load", msgQueue_.getQueueLoad()});
      }
    }
  }
  void startMessageProccesor() {
    while (run_) {
      auto msg = msgQueue_.pop();
      std::visit(MessageVisitor<T>{*this}, msg);
    }
  }
  friend struct MessageVisitor<T>;
  message::Queue<T> msgQueue_;
  std::map<std::string, T> assets_;
  std::mutex mtx_;
  std::atomic<bool> run_{true};
  Logger logger_;
  const MonitorResource *ms_;
  std::thread simulatorThread;
  std::thread messageProccessorThread;
};

template <typename T, typename... Args>
boost::shared_ptr<Server<T>> createServer(Args &&...args) {
  return boost::make_shared<Server<T>>(std::forward<Args>(args)...);
}
template <typename T> struct MessageVisitor {
  Server<T> &serv;
  void operator()(message::OrderRequest &o) {
    serv.logger_.log("Received order request", level::INFO,
                     field{trait::Print<T>::Header(), o.assetName},
                     field{"Order-type", o.getTypeStr()});
    message::OrderResponse resp(true);
    o.prom.set_value(resp);
  }

  void operator()(message::InfoRequest &i) {
    std::map<std::string, double> trends;
    int lookBackPeriod = trait::Trend<T>::LookBackPeriod();
    std::lock_guard<std::mutex> lock(serv.mtx_);
    if (i.assetSymbols.size() * lookBackPeriod < 1000) {
      trends = CalculateTrends(serv.assets_, i.assetSymbols, sequential{});
    } else {
      trends = CalculateTrends(serv.assets_, i.assetSymbols, parallel{});
    }

    message::InfoResponse resp;
    for (auto symbol : i.assetSymbols) {
      resp.assetInfos.emplace_back(
          symbol, serv.assets_.at(symbol).unitPriceOverTime_.back(),
          trends[symbol]);
    }
    i.prom.set_value(resp);
  }
  void operator()(message::Stop &s) { serv.run_ = false; }
};

template <> struct MessageVisitor<asset::Crypto> {
  Server<asset::Crypto> &serv;
  void operator()(message::OrderRequest &o) {
    serv.logger_.log("Received order request", level::INFO,
                     field{trait::Print<asset::Crypto>::Header(), o.assetName},
                     field{"Order-type", o.getTypeStr()});
    message::OrderResponse resp(true);
    o.prom.set_value(resp);
  }
  void operator()(message::InfoRequest &i) {
    std::map<std::string, double> trends;
    int lookBackPeriod = trait::Trend<asset::Crypto>::LookBackPeriod();
    std::lock_guard<std::mutex> lock(serv.mtx_);
    if (i.assetSymbols.size() * lookBackPeriod < 1000) {
      trends = CalculateTrends(serv.assets_, i.assetSymbols, sequential{});
    } else {
      trends = CalculateTrends(serv.assets_, i.assetSymbols, parallel{});
    }

    message::InfoResponse resp;
    for (auto symbol : i.assetSymbols) {
      resp.assetInfos.emplace_back(
          symbol, serv.assets_.at(symbol).unitPriceOverTime_.back(),
          trends[symbol]);
    }
    i.prom.set_value(resp);
  }
  void operator()(message::MineEvent &m) {
    std::lock_guard<std::mutex> lock(serv.mtx_);
    auto &crypto = serv.assets_.at(m.cryptoName);
    crypto.totalCoinsOnMarket += m.qty;
    double priceAffect = crypto.unitPriceOverTime_.back().value() * m.qty;
    crypto.unitPriceOverTime_.push_back(
        crypto.unitPriceOverTime_.back().value() - priceAffect);
  }
  void operator()(message::Stop &s) { serv.run_ = false; }
};
} // namespace exchange

#endif // EXCHANGE_SERVER_H
