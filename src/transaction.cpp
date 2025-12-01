<<<<<<< HEAD
<<<<<<< HEAD


#include "../include/transaction.h"

moneyTx::moneyTx(const int amount, moneyTxType type)
  : createdAt_(std::chrono::system_clock::now()), amount_(amount),
    type_(type) {
}

std::chrono::system_clock::time_point moneyTx::getCreatedAt() const {
  return createdAt_;
}

int moneyTx::getAmount() const { return amount_; }

moneyTxType moneyTx::getTransactionType() const { return type_; }

std::chrono::system_clock::time_point stockTx::getCreatedAt() const {
  return _createdAt;
}

int stockTx::getAmount() const {
  return amount_;
}

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


transaction::transaction(int amt, const std::string& t)
    : amount(amt), type(t), timestamp(std::chrono::system_clock::now()),
      details(std::pmr::polymorphic_allocator<string>(memRes)) {
}
=======
#include "transaction.hpp"
>>>>>>> b744aeb (commit)
=======
#include "transaction.hpp"

transaction::transaction(double amt, const string& t)
    : amount(amt), type(t), timestamp(std::chrono::system_clock::now()),
      details(std::pmr::polymorphic_allocator<string>(memRes)) {
}

double transaction::getAmount() const {
    return amount;
}

string transaction::getType() const {
    return type;
}

std::chrono::system_clock::time_point transaction::getTimestamp() const {
    return timestamp;
}

>>>>>>> b394ca4 (commit)
