
#include "exchange/currency/DKK.h"
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
  currency::DKK pricePerAsset_;
  currency::DKK total;
  std::chrono::system_clock::time_point createdAt_ =
      std::chrono::system_clock::now();
  std::string toString() const {
    return "Transaction type: " + assetHeader_ + " purchase" +
           "\nSymbol: " + assetName_ +
           "\nPrice per unit: " + pricePerAsset_.toString() +
           "\nquantity purchased: " + qty_;
  }
};
} // namespace exchange::tx
#endif // EXCHANGE_TX_PURCHASE_H
//
