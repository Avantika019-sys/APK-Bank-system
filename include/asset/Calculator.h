#include "asset/traits/Calculator.h"
#include <future>
#include <iostream>
#include <vector>
#ifndef BANK_CALCULATOR_H
#define BANK_CALCULATOR_H
namespace asset {
template <typename T> auto calculateTrendForIndividualAsset(const T asset) {
  typedef typename traits::Calculator<T>::AccT AccT;
  auto &vec = asset.priceOverTime;
  if (vec.size() == 1) {
    return AccT(0);
  }
  AccT sumX = 0;
  AccT sumY = 0;
  AccT sumXY = 0;
  AccT sumX2 = 0;
  if (traits::Calculator<T>::LookBackPeriod() > vec.size()) {
    for (int i = 0; i < vec.size(); i++) {
      int price = vec[i];
      sumX += i;
      sumY += price;
      sumXY += (i * price);
      sumX2 += (i * i);
    }
  } else {
    for (int i = vec.size() - 1;
         i > vec.size() - traits::Calculator<T>::LookBackPeriod(); i--) {
      int price = vec[i];
      sumX += i;
      sumY += price;
      sumXY += (i * price);
      sumX2 += (i * i);
    }
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
  using resType = decltype(calculateTrendForIndividualAsset<T>(assets[0]));
  std::vector<std::future<resType>> futures;
  for (auto &asset : assets) {
    futures.push_back(std::async(std::launch::async, [asset]() {
      return calculateTrendForIndividualAsset<T>(asset);
    }));
  }
  resType total = 0;
  for (auto &f : futures) {
    total += f.get();
  }
  return total / futures.size();
}
template <typename T>
auto CalculatePortfolioTrend(std::vector<T> assets, sequential) {
  using resType = decltype(calculateTrendForIndividualAsset<T>(assets[0]));
  resType total = 0;
  for (auto &asset : assets) {
    total += calculateTrendForIndividualAsset<T>(asset);
  }
  return total / assets.size();
}
template <typename T>
void CalculateDemandStatistics(std::string assetName, int qty,
                               int totalNoOfAssetForSale,
                               int totalNoOfAssetDemand, double price,
                               bool isBuy) {
  auto percentageOfMarketDemand = (qty / totalNoOfAssetDemand) * 100;
  auto percentageOfSale = (qty / totalNoOfAssetForSale) * 100;
  std::cout << percentageOfSale << percentageOfMarketDemand << std::endl;
};
} // namespace asset
#endif // BANK_CALCULATOR_H
