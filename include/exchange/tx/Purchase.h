
#include <chrono>
#include <memory_resource>
#include <string>
#ifndef EXCHANGE_TX_PURCHASE_H
#define EXCHANGE_TX_PURCHASE_H
namespace exchange::tx {
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
} // namespace exchange::tx
#endif // EXCHANGE_TX_PURCHASE_H
//
