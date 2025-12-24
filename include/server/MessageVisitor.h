#include "../asset/Calculator.h"
#include "Server.h"
namespace bank::server {
template <typename T> struct MessageVisitor {
  Server<T> &serv;
  void operator()(messages::OrderRequest<T> &o) {
    messages::OrderResponse resp(true);
    o.prom.set_value(resp);
  }
  void operator()(messages::InfoRequest<T> &i) {
    double trend = serv.calculateTrendForStock(i.stockName);
    int currentPrice = serv.stocks_[i.stockName].prices.back();
    i.prom.set_value(messages::InfoResponse<T>(currentPrice, trend));
  }
  void operator()(messages::PortfolioTrend<T> &p) {
    std::vector<T> assets;
    for (auto stock : p.ownedAsset) {
      assets.push_back(assets[stock]);
    }
    typedef typename AssetTraits<T>::AccT AccT;
    AccT trend;
    if (p.ownedAsset.size() * AssetTraits<T>::LookbackPeriod() < 1000) {
      trend = CalculatePortfolioTrend(assets, sequential{});
    } else {
      trend = CalculatePortfolioTrend(assets, parallel{});
    }
    p.prom.set_value(trend);
  }
  void operator()(messages::Stop &s) { serv.run = false; }
};
} // namespace bank::server
