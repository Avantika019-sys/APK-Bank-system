
#include "asset/traits/Print.h"
#include <chrono>
#include <memory_resource>
#include <string>
#ifndef BANK_TXASSETPURCHASE_H
#define BANK_TXASSETPURCHASE_H
namespace tx::asset {
struct Purchase {
  std::string assetName_;
  std::string assetHeader_;
  std::string qty_;
  double pricePerAsset_;
  double total;
  std::pmr::memory_resource *memRes_;
  std::chrono::system_clock::time_point createdAt_ =
      std::chrono::system_clock::now();
  std::string toString() const {
    return "Transaction type: " + assetHeader_ + " purchase" +
           "\nSymbol: " + assetName_ +
           "\nPrice per asset: " + std::to_string(pricePerAsset_) +
           "\nquantity purchased: " + qty_;
  }
};
} // namespace tx::asset
#endif // BANK_TXASSETPURCHASE_H
//
