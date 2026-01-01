#include "trait.hpp"
#include <future>
#include <vector>
#ifndef EXCHANGE_CALCULATOR_H
#define EXCHANGE_CALCULATOR_H
namespace exchange {
template <typename T> double calculateTrendForIndividualAsset(const T &asset) {
  typedef typename trait::Precision<T>::PrecisionT PrecisionT;
  auto &vec = asset.priceOverTime_;
  if (vec.size() == 1) {
    return PrecisionT{0};
  }
  PrecisionT sumX = 0;
  PrecisionT sumY = 0;
  PrecisionT sumXY = 0;
  PrecisionT sumX2 = 0;
  if (trait::Trend<T>::LookBackPeriod() > vec.size()) {
    for (int i = 0; i < vec.size(); i++) {
      int price = vec[i];
      sumX += i;
      sumY += price;
      sumXY += (i * price);
      sumX2 += (i * i);
    }
  } else {
    for (int i = vec.size() - 1;
         i > vec.size() - trait::Trend<T>::LookBackPeriod(); i--) {
      int price = vec[i];
      sumX += i;
      sumY += price;
      sumXY += (i * price);
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
auto CalculatePortfolioTrend(std::vector<T *> assets, parallel) {
  using resType = decltype(calculateTrendForIndividualAsset<T>(*assets[0]));
  std::vector<std::future<resType>> futures;
  for (auto &asset : assets) {
    futures.push_back(std::async(std::launch::async, [asset]() {
      return calculateTrendForIndividualAsset<T>(*asset);
    }));
  }
  resType total = 0;
  for (auto &f : futures) {
    total += f.get();
  }
  return total / futures.size();
}
template <typename T>
auto CalculatePortfolioTrend(std::vector<T *> assets, sequential) {
  using resType = decltype(calculateTrendForIndividualAsset<T>(*assets[0]));
  resType total = 0;
  for (auto asset : assets) {
    total += calculateTrendForIndividualAsset<T>(*asset);
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
  // std::cout << percentageOfSale << percentageOfMarketDemand << std::endl;
};
} // namespace exchange
#endif // EXCHANGE_CALCULATOR_H
