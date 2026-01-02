
#include "exchange/currency/DKK.h"
#include <chrono>
#include <memory_resource>
#include <string>
#ifndef EXCHANGE_TX_SALE_H
#define EXCHANGE_TX_SALE_H
namespace exchange::tx {
struct Sale {
  std::string assetSymbol_;
  std::string assetHeader_;
  std::string qty_;
  currency::DKK pricePerAsset_;
  currency::DKK total;
  std::pmr::memory_resource *memRes_;
  std::chrono::system_clock::time_point createdAt_ =
      std::chrono::system_clock::now();
  std::string toString() const {
    return "Transaction type: " + assetHeader_ +
           " sale\nSymbol: " + assetSymbol_ +
           "\nPrice per asset:" + pricePerAsset_.toString() +
           "\nQuantity sold: " + qty_;
  }
};
} // namespace exchange::tx
#endif // EXCHANGE_TX_SALE_H
