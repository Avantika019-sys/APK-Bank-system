
#include <chrono>
#include <memory_resource>
#include <string>
#ifndef BANK_TXASSETPURCHASE_H
#define BANK_TXASSETPURCHASE_H
namespace tx::asset {
struct Purchase {
  std::string assetName_;
  double qty_;
  double pricePerAsset_;
  double total;
  std::pmr::memory_resource *memRes_;
  std::chrono::system_clock::time_point createdAt_ =
      std::chrono::system_clock::now();
  std::string toString() const {
    return "Transaction type: asset purchase\nPrice per asset: " +
           std::to_string(pricePerAsset_) +
           "\nAmount of asset bought: " + std::to_string(qty_) +
           "\nStock name: " + assetName_;
  }
};
} // namespace tx::asset
#endif // BANK_TXASSETPURCHASE_H
