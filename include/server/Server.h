#include "./../asset/Crypto.h"
#include "./../asset/Stock.h"
#include "./../asset/traits.h"
#include "MessageQueue.h"
#include <boost/signals2/signal.hpp>
#include <random>
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H
namespace bank::server {
template <typename T> struct MessageVisitor;
template <typename T> class Server {
public:
  Server() : msgQueue_(10) {}

  static Server &getInstance() {
    static Server instance;
    return instance;
  }
  void addAsset(std::string name, T asset) { assets_[name] = asset; }
  StockUpdateSignal &getSignal(std::string assetName) {
    return assets_[assetName].signal;
  }
  void startSimulatingStockPriceUpdates() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distrib(-0.005, 0.005);

    while (run && !assets_.empty()) {
      std::this_thread::sleep_for(
          std::chrono::seconds(AssetTraits<T>::updateRate()));
      std::lock_guard<std::mutex> lock(mtx);
      for (auto &[assetName, asset] : assets_) {
        double percentChange = distrib(gen);
        double newPrice = asset.prices.back() * (1 + percentChange);
        asset.prices.push_back(newPrice);
        asset.signal(assetName, newPrice);
      }
    }
  }

  void startMessageProccesor() {
    while (run) {
      auto msg = msgQueue_.pop();
      std::visit(MessageVisitor<T>{*this}, msg);
    }
  }
  void pushMsg(Message<T> &&msg) { msgQueue_.push(std::move(msg)); }

private:
  friend struct MessageVisitor<T>;
  MessageQueue<T> msgQueue_;
  std::map<std::string, T> assets_;
  std::mutex mtx;
  std::atomic<bool> run{true};
};
} // namespace bank::server

#endif // BANK_STOCKSERVER_H
