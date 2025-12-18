
#include "transaction.h"
#include <iostream>

Tx::Tx(details d) : details_(d), createdAt_(std::chrono::system_clock::now()) {}

std::chrono::system_clock::time_point Tx::getCreatedAt() const {
  return createdAt_;
}

const details &Tx::getDetails() const { return details_; }

std::ostream &operator<<(std::ostream &os, const Tx &t) {
  std::string detailsStr = std::visit(ToString{}, t.getDetails());
  os << "Transaction created at: " << t.getCreatedAt() << detailsStr
     << std::endl;
  return os;
}

// std::string Tx::toString() { return ""; }
// std::ostream &operator<<(std::ostream &os, const stockTx &t) {
//   os << "Transaction created at: " << t.getCreatedAt()
//      << " Amount: " << t.getAmount() << std::endl;
//   return os;
// }

// std::chrono::system_clock::time_point stockTx::getCreatedAt() const {
//   return createdAt_;
// }
// int stockTx::getAmount() const { return amount_; }
// stockTx::stockTx() {}
//
// stockTx::stockTx(int amount, std::string stockName)
//     : amount_(amount), stockName_(stockName) {}
