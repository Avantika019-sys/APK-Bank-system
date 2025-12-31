
#include <chrono>
#include <memory_resource>
#include <string>
#ifndef BANK_TXASSETSELL_H
#define BANK_TXASSETSELL_H
namespace tx::asset {
struct Sale {
  std::string assetSymbol_;
  std::string assetHeader_;
  double qty_;
  double pricePerAsset_;
  double total;
  std::pmr::memory_resource *memRes_;
  std::chrono::system_clock::time_point createdAt_ =
      std::chrono::system_clock::now();
  std::string toString() const {
    return "Transaction type: " + assetHeader_ +
           " sale\nSymbol: " + assetSymbol_ +
           "\nPrice per asset:" + std::to_string(pricePerAsset_) +
           "\nQuantity sold: " + std::to_string(qty_);
  }
};
} // namespace tx::asset
#endif // BANK_TXASSETPURCHASE_H
