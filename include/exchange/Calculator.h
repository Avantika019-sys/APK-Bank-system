#include "exchange/message.hpp"
#include "trait.hpp"
#include <execution>
#include <future>
#include <queue>
#include <stack>
#include <thread>
#include <unordered_set>
#include <vector>
#ifndef EXCHANGE_CALCULATOR_H
#define EXCHANGE_CALCULATOR_H
namespace exchange {
template <typename T> double calculateTrendForIndividualAsset(const T *asset) {
  typedef typename trait::Precision<T>::PrecisionT PrecisionT;
  const auto &vec = asset->unitPriceOverTime_;
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
std::map<std::string, double> CalculateTrends(std::vector<const T *> assets,
                                              parallel) {
  std::vector<double> res(assets.size());
  auto cores = std::thread::hardware_concurrency();

  auto func = [&](int i) {
    while (i < assets.size()) {
      double trend = calculateTrendForIndividualAsset(assets[i]);
      res[i] = trend;
      i += cores;
    }
  };
  std::vector<std::thread> threads;
  for (int i = 0; i < cores; i++) {
    threads.push_back(std::thread(func, i));
  }
  for (auto &thread : threads) {
    thread.join();
  }

  std::map<std::string, double> trends;
  for (int i = 0; i < res.size(); i++) {
    trends.emplace(assets[i]->symbol, res[i]);
  }
  return trends;
}
template <typename T>
std::map<std::string, double> CalculateTrends(std::vector<const T *> Assets,
                                              sequential) {
  std::map<std::string, double> trends;
  std::transform(Assets.begin(), Assets.end(),
                 std::inserter(trends, trends.end()),
                 [&trends](const T *asset) {
                   double trend = calculateTrendForIndividualAsset(asset);
                   return std::make_pair(asset->symbol, trend);
                 });
  return trends;
}
} // namespace exchange
#endif // EXCHANGE_CALCULATOR_H
