//
// Created by ali on 10/30/25.
//

#include "../include/transaction.h"

moneyTx::moneyTx(const int amount, moneyTxType type)
    : createdAt_(std::chrono::system_clock::now()), amount_(amount),
      type_(type) {}

std::chrono::system_clock::time_point moneyTx::getCreatedAt() const {
  return createdAt_;
}

int moneyTx::getAmount() const { return amount_; }

moneyTxType moneyTx::getTransactionType() const { return type_; }

std::ostream &operator<<(std::ostream &os, const moneyTx &t) {
  std::string transactionTypeStr = "withdrawal";
  if (t.getTransactionType() == moneyTxType::deposit) {
    transactionTypeStr = "deposit";
  }
  os << "Transaction created at: " << t.getCreatedAt()
     << " Amount: " << t.getAmount() << " Type: " << transactionTypeStr
     << std::endl;
  return os;
}

std::ostream &operator<<(std::ostream &os, const stockTx &t) {
  os << "Transaction created at: " << t.getCreatedAt()
     << " Amount: " << t.getAmount() << std::endl;
  return os;
}
