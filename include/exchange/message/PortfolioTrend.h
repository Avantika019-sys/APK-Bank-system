#include <future>
#include <set>
#include <unordered_set>
#ifndef EXCHANGE_MESSAGE_PORTFOLIOTREND_H
#define EXCHANGE_MESSAGE_PORTFOLIOTREND_H
namespace exchange::message {
template <typename T> struct PortfolioTrendRequest {
  std::unordered_set<std::string> ownedAssets;
  std::promise<double> prom;
};
} // namespace exchange::message
#endif // EXCHANGE_MESSAGE_PORTFOLIOTREND_H
