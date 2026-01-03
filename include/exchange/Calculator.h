#include "trait.hpp"
#include <future>
#include <unordered_set>
#include <vector>
#ifndef EXCHANGE_CALCULATOR_H
#define EXCHANGE_CALCULATOR_H
namespace exchange {
template <typename T> double calculateTrendForIndividualAsset(const T &asset) {
  typedef typename trait::Precision<T>::PrecisionT PrecisionT;
  const auto &vec = asset.unitPriceOverTime_;
  if (vec.size() == 1) {
    return 0;
  }
  PrecisionT sumX = 0;
  PrecisionT sumY = 0;
  PrecisionT sumXY = 0;
  PrecisionT sumX2 = 0;
  if (trait::Trend<T>::LookBackPeriod() > vec.size()) {
    for (int i = 0; i < vec.size(); i++) {
      currency::DKK price = vec[i];
      sumX += i;
      sumY += price.value();
      sumXY += (i * price.value());
      sumX2 += (i * i);
    }
  } else {
    for (int i = vec.size() - 1;
         i > vec.size() - trait::Trend<T>::LookBackPeriod(); i--) {
      currency::DKK price = vec[i];
      sumX += i;
      sumY += price.value();
      sumXY += (i * price.value());
      sumX2 += (i * i);
    }
  }
  PrecisionT numerator = (vec.size() * sumXY) - (sumX * sumY);
  PrecisionT denominator = (vec.size() * sumX2) - (sumX * sumX);
  PrecisionT m = numerator / denominator;
  PrecisionT b = (sumY - m * sumX) / vec.size();
  PrecisionT totalChange = m * (vec.size() - 1);
  return (totalChange / b) * 100;
}
struct parallel {};
struct sequential {};
template <typename T>
std::map<std::string, double>
CalculateTrends(const std::map<std::string, T> &assets,
                const std::vector<std::string> &ownedAssets, parallel) {
  std::map<std::string, double> trends;
  std::vector<std::future<double>> futures;
  for (const auto &[symbol, asset] : assets) {
    auto it = std::find(ownedAssets.begin(), ownedAssets.end(), symbol);
    if (it != ownedAssets.end()) {
      futures.push_back(std::async(std::launch::async, [&asset]() {
        return calculateTrendForIndividualAsset<T>(asset);
      }));
    }
  }
  for (int i = 0; i < futures.size(); i++) {
    double trend = futures[i].get();
    trends[ownedAssets[i]] = trend;
  }
  return trends;
}
template <typename T>
std::map<std::string, double>
CalculateTrends(const std::map<std::string, T> &assets,
                const std::vector<std::string> &ownedAssets, sequential) {
  std::map<std::string, double> trends;
  for (const auto &[symbol, asset] : assets) {
    auto it = std::find(ownedAssets.begin(), ownedAssets.end(), symbol);
    if (it != ownedAssets.end()) {
      double trend = calculateTrendForIndividualAsset<T>(asset);
      trends[symbol] = trend;
    }
  }
  return trends;
}
} // namespace exchange
#endif // EXCHANGE_CALCULATOR_H
