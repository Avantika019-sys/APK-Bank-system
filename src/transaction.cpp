
#include "transaction.h"
#include <iostream>

moneyTx::moneyTx(double amt, const string &t)
    : amount(amt), type(t), timestamp(std::chrono::system_clock::now()),
      details(std::pmr::polymorphic_allocator<string>(memRes)) {}

std::chrono::system_clock::time_point moneyTx::getCreatedAt() const {
  return createdAt_;
}

uint moneyTx::getAmount() const { return amount_; }

std::ostream &operator<<(std::ostream &os, const moneyTx &t) {
  std::string transactionTypeStr = "withdrawal";
  if (t.getType() == moneyTxType::deposit) {
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

int stockTx::getAmount() const { return amount_; }
moneyTxType moneyTx::getType() const { return type_; }
