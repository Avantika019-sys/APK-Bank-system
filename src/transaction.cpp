
#include "transaction.h"
#include <iostream>



Tx::Tx(uint amt, TxType type, std::pmr::memory_resource* memRes)
    : memRes_(memRes), details_(memRes), amount_(amt), type_(type), createdAt_(std::chrono::system_clock::now()){}

std::chrono::system_clock::time_point Tx::getCreatedAt() const {
  return createdAt_;
}

uint Tx::getAmount() const { return amount_; }

std::ostream &operator<<(std::ostream &os, const Tx &t) {
  std::string transactionTypeStr = "withdrawal";
  if (t.getType() == TxType::deposit) {
    transactionTypeStr = "deposit";
  }
  if (t.getType() == TxType::stockPurchase) {
    transactionTypeStr = "stock purchase";
  }
  os << "Transaction created at: " << t.getCreatedAt()
     << " Amount: " << t.getAmount() << " Type: " << transactionTypeStr
     << std::endl;
  return os;
}

TxType Tx::getType() const { return type_; }
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
