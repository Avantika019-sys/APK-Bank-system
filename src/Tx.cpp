
#include "Tx.h"
#include <iostream>

Tx::Tx(uint amt, TxType type, std::pmr::memory_resource *memRes)
    : memRes_(memRes), details_(memRes), amount_(amt), type_(type),
      createdAt_(std::chrono::system_clock::now()) {}

std::chrono::system_clock::time_point Tx::getCreatedAt() const {
  return createdAt_;
}

const details &Tx::getDetails() const { return details_; }

std::ostream &operator<<(std::ostream &os, const Tx &t) {
  struct ToString {
    std::string operator()(const stockSellDetails &arg) {
      return "transaction type: stock sale\nPrice per stock: " +
             std::to_string(arg.pricePerStock_) +
             "\nAmount of stocks sold: " + std::to_string(arg.pricePerStock_) +
             "\nStock name: " + arg.stockName_;
    }
    std::string operator()(stockPurchaseDetails arg) {
      return "transaction type: stock purchase\nPrice per stock: " +
             std::to_string(arg.pricePerStock_) +
             "\nAmount of stocks bought: " + std::to_string(arg.stocksBought_) +
             "\nStock name: " + arg.stockName_;
    }
    std::string operator()(withdrawDetails arg) {
      return "transaction type: withdrawal\nAmount withdrawn: " +
             std::to_string(arg.amountWithdrawn_);
    }
    std::string operator()(depositDetails arg) {
      return "transaction type: deposit\nAmount depositted: " +
             std::to_string(arg.amountDepositted_);
    }
  };
  std::string detailsStr = std::visit(ToString{}, t.getDetails());
  os << "Transaction created at: " << t.getCreatedAt() << detailsStr
     << std::endl;
  return os;
}
