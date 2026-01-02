#include <future>
#include <map>
#include <vector>
#ifndef EXCHANGE_MESSAGE_PORTFOLIOTREND_H
#define EXCHANGE_MESSAGE_PORTFOLIOTREND_H
namespace exchange::message {
template <typename T> struct PortfolioTrendRequest {
  std::vector<std::string> ownedAssets;
  std::promise<std::map<std::string, double>> prom;
};
} // namespace exchange::message
#endif // EXCHANGE_MESSAGE_PORTFOLIOTREND_H
