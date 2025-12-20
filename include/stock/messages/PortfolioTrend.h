#ifndef BANK_PORTFOLIOTREND_H
#define BANK_PORTFOLIOTREND_H
#include <future>
#include <set>
namespace bank::stock::messages {
struct PortfolioTrend {
  PortfolioTrend(std::set<std::string> ownedStocks)
      : ownedStocks(ownedStocks) {}
  std::set<std::string> ownedStocks;
  std::promise<double> prom;
};
}      // namespace bank::stock::messages
#endif // BANK_PORTFOLIOTREND_H
