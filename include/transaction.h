#ifndef BANK_TRANSACTION_H
#define BANK_TRANSACTION_H
#include <chrono>

enum class moneyTxType {
  deposit,
  withdraw,
  stockPurchase,
};

class moneyTx {
public:
  moneyTx();

  moneyTx(int amount, moneyTxType type);

  std::chrono::system_clock::time_point getCreatedAt() const;

  int getAmount() const;

  moneyTxType getTransactionType() const;

private:
  std::chrono::system_clock::time_point createdAt_;
  int amount_;
  moneyTxType type_;
};

class stockTx {
public:
  stockTx();

  stockTx(int amount, std::string stockName);

  std::chrono::system_clock::time_point getCreatedAt() const;

  int getAmount() const;

private:
  std::chrono::system_clock::time_point _createdAt;
  int amount_;
};

std::ostream &operator<<(std::ostream &os, const moneyTx &t);
std::ostream &operator<<(std::ostream &os, const stockTx &t);

#endif // BANK_TRANSACTION_H
