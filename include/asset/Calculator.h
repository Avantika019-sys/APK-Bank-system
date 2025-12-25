#include "traits.h"
#include <future>
#include <vector>
#ifndef BANK_CALCULATOR_H
#define BANK_CALCULATOR_H
template <typename T> auto calculateTrendForIndividualAsset(T asset) {
  typedef typename AssetTraits<T>::AccT AccT;
  auto &vec = asset.priceOverTime;
  AccT sumX = 0;
  AccT sumY = 0;
  AccT sumXY = 0;
  AccT sumX2 = 0;
  for (int i = vec.size() - 1;
       i > vec.size() - AssetTraits<T>::LookBackPeriod(); i--) {
    int price = vec[i];
    sumX += i;
    sumY += price;
    sumXY += (i * price);
    sumX2 += (i * i);
  }
  AccT numerator = (vec.size() * sumXY) - (sumX * sumY);
  AccT denominator = (vec.size() * sumX2) - (sumX * sumX);
  AccT m = numerator / denominator;
  AccT b = (sumY - m * sumX) / vec.size();
  AccT totalChange = m * (vec.size() - 1);
  return (totalChange / b) * 100;
}
struct parallel {};
struct sequential {};
template <typename T>
auto CalculatePortfolioTrend(std::vector<T> assets, parallel) {
  std::vector<std::future<double>> futures;
  for (auto asset : assets) {
    futures.push_back(std::async(std::launch::async,
                                 calculateTrendForIndividualAsset, asset));
  }
  double total = 0;
  for (auto &f : futures) {
    total += f.get();
  }
  return total / futures.size();
}
template <typename T>
auto CalculatePortfolioTrend(std::vector<T> assets, sequential) {
  double total = 0;
  for (auto asset : assets) {
    total += calculateTrendForIndividualAsset(asset);
  }
  return total / assets.size();
}
#endif // BANK_CALCULATOR_H
