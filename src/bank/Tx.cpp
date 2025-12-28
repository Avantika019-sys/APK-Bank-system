
#include "bank/Tx.h"
#include <iostream>

namespace bank {
Tx::Tx(details d, std::pmr::memory_resource *memRes)
    : memRes_(memRes), createdAt_(std::chrono::system_clock::now()) {}

std::chrono::system_clock::time_point Tx::getCreatedAt() const {
  return createdAt_;
}

const details &Tx::getDetails() const { return details_; }

std::ostream &operator<<(std::ostream &os, const Tx &t) {
  struct ToString {
    std::string operator()(const assetSellDetails &arg) {
      return " transaction type: asset sale\nPrice per asset: " +
             std::to_string(arg.pricePerAsset_) +
             "\nAmount of stocks sold: " + std::to_string(arg.pricePerAsset_) +
             "\nStock name: " + arg.assetName_;
    }
    std::string operator()(assetPurchaseDetails arg) {
      return " transaction type: stock purchase\nPrice per stock: " +
             std::to_string(arg.pricePerAsset_) +
             "\nAmount of stocks bought: " + std::to_string(arg.assetsBought_) +
             "\nStock name: " + arg.assetName_;
    }
    std::string operator()(withdrawDetails arg) {
      return " transaction type: withdrawal\nAmount withdrawn: " +
             std::to_string(arg.amountWithdrawn_);
    }
    std::string operator()(depositDetails arg) {
      return " transaction type: deposit\nAmount depositted: " +
             std::to_string(arg.amountDepositted_);
    }
  };
  std::string detailsStr = std::visit(ToString{}, t.getDetails());
  os << "Transaction created at: " << t.getCreatedAt() << detailsStr
     << std::endl;
  return os;
}
} // namespace bank
