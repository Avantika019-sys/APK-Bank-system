#include "trait.hpp"
#include <thread>
#include <vector>
#ifndef EXCHANGE_CALCULATOR_H
#define EXCHANGE_CALCULATOR_H
namespace exchange {
template <typename T> double calculateAssetTrend(const T *asset) {
  const auto &vec = asset->unitPriceOverTime_;
  int size = vec.size();
  if (size == 1) {
    return 0;
  }
  double sumX = 0;
  double sumY = 0;
  double sumXY = 0;
  double sumX2 = 0;
  if (trait::Trend<T>::LookBackPeriod() > size) {
    for (int i = 0; i < size; i++) {
      currency::DKK price = vec[i];
      sumX += i;
      sumY += price.value();
      sumXY += (i * price.value());
      sumX2 += (i * i);
    }
  } else {
    for (int i = size - 1; i > size - trait::Trend<T>::LookBackPeriod(); i--) {
      currency::DKK price = vec[i];
      sumX += i;
      sumY += price.value();
      sumXY += (i * price.value());
      sumX2 += (i * i);
    }
  }
  double numerator = (size * sumXY) - (sumX * sumY);
  double denominator = (size * sumX2) - (sumX * sumX);
  double m = numerator / denominator;
  double b = (sumY - m * sumX) / size;
  double totalChange = m * (size - 1);
  return (totalChange / b) * 100;
}
struct parallel {};
struct sequential {};
template <typename T>
std::map<std::string, double> CalculateTrends(std::vector<const T *> assets,
                                              parallel) {
  std::map<std::string, double> trends;
  std::vector<std::future<double>> futures;
  for (const auto &asset : assets) {
    futures.push_back(std::async(std::launch::async,
                                 calculateAssetTrend<T>, asset));
  };

  for (int i = 0; i < futures.size(); i++) {
    double trend = futures[i].get();
    trends[assets[i]->symbol] = trend;
  }
  return trends;
}
template <typename T>
std::map<std::string, double> CalculateTrends(std::vector<const T *> Assets,
                                              sequential) {
  std::map<std::string, double> trends;
  std::transform(Assets.begin(), Assets.end(),
                 std::inserter(trends, trends.end()), [](const T *asset) {
                   double trend = calculateAssetTrend(asset);
                   return std::make_pair(asset->symbol, trend);
                 });
  return trends;
}
} // namespace exchange
#endif // EXCHANGE_CALCULATOR_H
