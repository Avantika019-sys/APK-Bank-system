#include "Calculator.h"
#include "OrderArray.h"
#include "asset.hpp"
#include "exchange/currency/DKK.h"
#include "exchange/message/Info.h"
#include "exchange/util/observability.hpp"
#include "message.hpp"
#include "trait.hpp"
#include <algorithm>
#include <boost/signals2/connection.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/smart_ptr/make_shared_array.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <concepts>
#include <exception>
#include <functional>
#include <iostream>
#include <random>
#include <thread>
#include <type_traits>
#include <utility>
#ifndef EXCHANGE_SERVER_H
#define EXCHANGE_SERVER_H
using namespace std::chrono_literals;
using namespace exchange::util::observability;
namespace exchange {

template <typename T>
concept hasGetLatestPrice = requires(T asset) {
  { asset.getLatestPrice() } -> std::same_as<currency::DKK>;
};

template <typename T> struct MessageVisitor;

template <typename T> class Server {
public:
  Server(std::string logfile, const MonitorResource &ms) try
      : msgQueue_(trait::MessageQueue<T>::QueueCapacity()), ms_(ms),
        orders_(10), ordersSnapshot_(10), logger_(logfile) {
  } catch (std::exception &e) {
    std::cout << "Error creating " << trait::Print<T>::Header()
              << " Server:" << e.what() << std::endl;
    throw;
  }

  void start() {
    simulatorThread =
        std::thread([this]() { startSimulatingAssetPriceUpdates(); });
    messageProccessorThread =
        std::thread([this]() { startMessageProccesor(); });
  }
  void addAsset(std::string symbol, T asset) {
    std::lock_guard<std::mutex> lock(mtx_);
    assets_.try_emplace(std::move(symbol), std::move(asset));
  }

  boost::signals2::connection
  subscribeToPriceUpdates(std::string assetSymbol,
                          std::function<void(currency::DKK UpdatedPrice)> cb) {
    std::lock_guard<std::mutex> lock(mtx_);
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
  OrderArray getOrderHistoryForAsset(std::string symbol) {
    std::lock_guard<std::mutex> lock(snapshotMtx_);
    OrderArray res(ordersSnapshot_.size_);
    std::remove_copy_if(
        ordersSnapshot_.orders_,
        ordersSnapshot_.orders_ + ordersSnapshot_.currentOrders_, res.orders_,
        [&symbol](const Order &o) { return o.assetSymbol != symbol; });
    return res;
  }

private:
  void startSimulatingAssetPriceUpdates() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(0.1);
    static_assert(hasGetLatestPrice<T>);
    while (run_) {
      std::this_thread::sleep_for(
          std::chrono::seconds(trait::Server<T>::UpdateRate()));
      std::lock_guard<std::mutex> lock(mtx_);
      for (auto &[symbol, asset] : assets_) {
        auto newPrice = asset.getLatestPrice();
        asset.unitPriceOverTime_.push_back(newPrice);
        (*asset.sig_)(newPrice);
      }
      if (d(gen)) {
        logger_.log(
            "system health", util::observability::level::INFO,
            util::observability::field("bytes", ms_.getbytesalloc()),
            util::observability::field{"queue-load", msgQueue_.getQueueLoad()});

        std::lock_guard<std::mutex> lock(snapshotMtx_);
        try {
          ordersSnapshot_ = orders_;
        } catch (...) {
          logger_.log("failed to update snapshot",
                      util::observability::level::ERROR);
        }
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
  std::unordered_map<std::string, T> assets_;
  std::mutex mtx_;
  std::mutex snapshotMtx_;
  std::atomic<bool> run_{true};
  Logger logger_;
  const MonitorResource &ms_;
  OrderArray orders_;
  OrderArray ordersSnapshot_;
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
                     field{trait::Print<T>::Header(), o.order.assetSymbol},
                     field{"order", o.order});
    serv.orders_.addOrder(o.order);
    message::OrderResponse resp(true);
    o.prom.set_value(resp);
  }

  void operator()(message::InfoRequest &i) {
    std::map<std::string, double> trends;
    int lookBackPeriod = trait::Trend<T>::LookBackPeriod();
    std::lock_guard<std::mutex> lock(serv.mtx_);
    std::vector<const T *> filteredAssets;

    for (const auto &symbol : i.assetSymbols) {
      auto it = serv.assets_.find(symbol);
      if (it != serv.assets_.end()) {
        filteredAssets.push_back(&it->second);
      }
    }

    if (i.assetSymbols.size() * lookBackPeriod < 1000) {
      trends = CalculateTrends(filteredAssets, sequential{});
    } else {
      trends = CalculateTrends(filteredAssets, parallel{});
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

} // namespace exchange

#endif // EXCHANGE_SERVER_H
