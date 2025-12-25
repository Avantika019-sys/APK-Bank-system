#ifndef BANK_PORTFOLIOTREND_H
#define BANK_PORTFOLIOTREND_H
#include <future>
#include <set>
namespace bank::server::messages {
template <typename T> struct PortfolioTrend {
  PortfolioTrend(std::set<std::string> ownedStocks) : ownedAsset(ownedStocks) {}
  std::set<std::string> ownedAsset;
  std::promise<double> prom;
};
} // namespace bank::server::messages
#endif // BANK_PORTFOLIOTREND_H
