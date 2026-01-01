#include <future>
#include <set>
#include <unordered_set>
#ifndef BANK_PORTFOLIOTREND_H
#define BANK_PORTFOLIOTREND_H
namespace asset::message::types {
template <typename T> struct PortfolioTrendRequest {
  std::unordered_set<std::string> ownedAssets;
  std::promise<double> prom;
};
} // namespace asset::message::types
#endif // BANK_PORTFOLIOTREND_H
