
#include <chrono>
#include <memory_resource>
#include <string>
#ifndef BANK_TXASSETSELL_H
#define BANK_TXASSETSELL_H
namespace tx::asset {
struct Sale {
  std::string assetName_;
  double qty_;
  double pricePerAsset_;
  double total;
  std::pmr::memory_resource *memRes_;
  std::chrono::system_clock::time_point createdAt_ =
      std::chrono::system_clock::now();
  std::string toString() const {
    return "Transaction type: asset sale\nPrice per asset: " +
           std::to_string(pricePerAsset_) +
           "\nAmount of asset sold: " + std::to_string(pricePerAsset_) +
           "\nStock name: " + assetName_;
  }
};
} // namespace tx::asset
  // Tx(details d, std::pmr::memory_resource *memRes);
#endif // BANK_TXASSETPURCHASE_H
