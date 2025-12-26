#include "./../asset/Calculator.h"
#include "./../asset/Crypto.h"
#include "./../asset/Stock.h"
#include "./../asset/traits.h"
#include "MessageQueue.h"
#include <boost/signals2/signal.hpp>
#include <memory>
#include <random>
#include <thread>
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H
namespace bank::server {
typedef boost::signals2::signal<void(std::string assetName,
                                     double UpdatedPrice)>
    UpdateSignal;
template <typename T> struct MessageVisitor;
template <typename T> class Server {
public:
  Server() : msgQueue_(10) {
    std::thread stockUpdaterThread(
        [&]() { startSimulatingAssetPriceUpdates(); });
    std::thread stockOrderThread([&]() { startMessageProccesor(); });
  }

  static Server &getInstance() {
    static Server instance;
    return instance;
  }
  void addAsset(std::string name, T asset) { assets_[name] = asset; }
  UpdateSignal &getSignal(std::string assetName) {
    return assets_[assetName].second;
  }
  void startSimulatingAssetPriceUpdates() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distrib(-0.005, 0.005);

    while (run && !assets_.empty()) {
      std::this_thread::sleep_for(
          std::chrono::seconds(AssetTraits<T>::updateRate()));
      std::lock_guard<std::mutex> lock(mtx);
      for (auto &[assetName, asset] : assets_) {
        double percentChange = distrib(gen);
        double newPrice =
            asset.first.priceOverTime.back() * (1 + percentChange);
        asset.first.priceOverTime.push_back(newPrice);
        asset.second(assetName, newPrice);
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
  std::map<std::string, std::pair<T, UpdateSignal>> assets_;
  std::mutex mtx;
  std::atomic<bool> run{true};
  std::thread simulatorThread;
  std::thread messageProccessorThread;
};
template <typename T> struct MessageVisitor {
  Server<T> &serv;
  void operator()(messages::OrderRequest<T> &o) {
    messages::OrderResponse resp(true);
    o.prom.set_value(resp);
  }
  void operator()(messages::InfoRequest<T> &i) {
    // double trend = serv.calculateTrendForStock(i.assetName);
    int currentPrice = serv.assets_[i.assetName].first.priceOverTime.back();
    i.prom.set_value(messages::InfoResponse<T>(currentPrice, 1.0));
  }
  void operator()(messages::PortfolioTrend<T> &p) {
    std::vector<T> assets;
    for (auto assetName : p.ownedAsset) {
      assets.push_back(serv.assets_[assetName].first);
    }
    typedef typename AssetTraits<T>::AccT AccT;
    AccT trend;
    if (p.ownedAsset.size() * AssetTraits<T>::LookBackPeriod() < 1000) {
      trend = CalculatePortfolioTrend(assets, sequential{});
    } else {
      trend = CalculatePortfolioTrend(assets, parallel{});
    }
    p.prom.set_value(trend);
  }
  void operator()(messages::Stop &s) { serv.run = false; }
};
} // namespace bank::server

#endif // BANK_STOCKSERVER_H
