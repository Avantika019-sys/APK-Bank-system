#include "asset/Calculator.h"
#include "asset/message/Queue.h"
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
        logger_(std::move(logger)) {

    simulatorThread =
        std::thread([this]() { startSimulatingAssetPriceUpdates(); });
    messageProccessorThread =
        std::thread([this]() { startMessageProccesor(); });
  }

  void addAsset(std::string &&symbol, T &&asset) {
    std::lock_guard<std::mutex> lock(mtx_);
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
    while (run_) {
      std::this_thread::sleep_for(
          std::chrono::seconds(traits::Server<T>::UpdateRate()));
      std::lock_guard<std::mutex> lock(mtx_);
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
    pushMsg(message::types::Stop());
    if (simulatorThread.joinable())
      simulatorThread.join();
    if (messageProccessorThread.joinable())
      messageProccessorThread.join();
  }

private:
  friend struct MessageVisitor<T>;
  message::Queue<T> msgQueue_;
  std::map<std::string, T> assets_;
  std::mutex mtx_;
  std::atomic<bool> run_{true};
  std::vector<std::function<void(
      std::string assetName, int qty, int totalNoOfAssetForSale,
      int totalNoOfAssetDemand, double price, bool isBuy)>>
      OrderEventCbs;
  std::unique_ptr<util::Logger> logger_;
  std::thread simulatorThread;
  std::thread messageProccessorThread;
};
} // namespace asset

#endif // BANK_STOCKSERVER_H
