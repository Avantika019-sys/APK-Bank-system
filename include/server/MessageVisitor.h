#include "Server.h"
#include "asset/Calculator.h"
namespace bank::server {
struct MessageVisitor {
  Server &serv;
  void operator()(messages::OrderRequest &o) {
    messages::OrderResponse resp(true);
    o.prom.set_value(resp);
  }
  void operator()(messages::InfoRequest &i) {
    double trend = serv.calculateTrendForStock(i.stockName);
    int currentPrice = serv.stocks_[i.stockName].prices.back();
    i.prom.set_value(messages::InfoResponse(currentPrice, trend));
  }
  void operator()(messages::PortfolioTrend &p) {
    int totalDataPoints = 0;
    for (auto stock : p.ownedStocks) {
      totalDataPoints += serv.stocks_[stock].prices.size();
    }
    double trend = 0;
    if (totalDataPoints > 1000) {
      CalculatePortfolioTrend(, sequential);
    } else {
    }
    p.prom.set_value(trend);
  }
  void operator()(messages::Stop &s) { serv.run = false; }
};
} // namespace bank::server
