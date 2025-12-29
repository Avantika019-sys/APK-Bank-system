#include "Calculator.h"
#include "MessageQueue.h"
#include "Traits.h"
#include "asset/messages/Stop.h"
#include "types/Crypto.h"
#include "types/Stock.h"
#include <boost/signals2/signal.hpp>
#include <memory>
#include <random>
#include <thread>
#include <tuple>
#include <utility>
#ifndef BANK_STOCKSERVER_H
#define BANK_STOCKSERVER_H
namespace asset {
typedef boost::signals2::signal<void(std::string assetName,
                                     double UpdatedPrice)>
    UpdateSignal;
template <typename T> struct MessageVisitor;
template <typename T> class Server {
public:
  Server() : msgQueue_(Traits<T>::QueueCapacity()) {

    simulatorThread =
        std::thread([this]() { startSimulatingAssetPriceUpdates(); });
    messageProccessorThread =
        std::thread([this]() { startMessageProccesor(); });
  }

  void addAsset(std::string symbol, T asset) {
    std::lock_guard<std::mutex> lock(mtx);
    assets_.emplace(
        std::piecewise_construct, std::forward_as_tuple(symbol),
        std::forward_as_tuple(asset, std::make_shared<UpdateSignal>()));
  }
  UpdateSignal &getSignal(std::string assetName) {
    return *(assets_.at(assetName).second);
  }
  void startSimulatingAssetPriceUpdates() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distrib(-0.005, 0.005);

    while (run) {
      std::this_thread::sleep_for(
          std::chrono::seconds(Traits<T>::updateRate()));
      std::lock_guard<std::mutex> lock(mtx);
      for (auto &[symbol, asset] : assets_) {
        double percentChange = distrib(gen);
        double newPrice =
            asset.first.priceOverTime.back() * (1 + percentChange);
        asset.first.priceOverTime.push_back(newPrice);
        (*asset.second)(symbol, newPrice);
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
  std::map<std::string, std::pair<T, std::shared_ptr<UpdateSignal>>> assets_;
  std::mutex mtx;
  std::atomic<bool> run{true};
  std::vector<std::function<void(std::string assetName, int qty)>>
      OrderEventCbs;
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
    auto asset = serv.assets_.at(i.assetName).first;
    auto trend = calculateTrendForIndividualAsset<T>(asset);
    int price = serv.assets_[i.assetName].first.priceOverTime.back();
    i.prom.set_value(messages::InfoResponse<T>(price, trend));
  }
  void operator()(messages::PortfolioTrend<T> &p) {
    std::vector<T> assets;
    for (auto assetName : p.ownedAsset) {
      assets.push_back(serv.assets_.at(assetName).first);
    }
    typedef typename Traits<T>::AccT AccT;
    AccT trend;
    if (p.ownedAsset.size() * Traits<T>::LookBackPeriod() < 1000) {
      trend = CalculatePortfolioTrend(assets, sequential{});
    } else {
      trend = CalculatePortfolioTrend(assets, parallel{});
    }
    p.prom.set_value(trend);
  }
  void operator()(messages::Stop &s) { serv.run = false; }
  void operator()(messages::OrderEvent &o) {
    serv.OrderEventCbs.push_back(o.cb);
  }
};
} // namespace asset

#endif // BANK_STOCKSERVER_H
