
#ifndef BANK_TRANSACTION_H
#define BANK_TRANSACTION_H
#include <chrono>
#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>
using namespace std;

enum class moneyTxType {
  deposit,
  withdraw,
  stockPurchase,
};

// enum stock, buy sell

class moneyTx {

public:
  moneyTx(uint amt, moneyTxType type);
  uint getAmount() const;
  moneyTxType getType() const;
  std::chrono::system_clock::time_point getCreatedAt() const;

private:
  std::pmr::memory_resource *memRes = std::pmr::get_default_resource();
  std::pmr::vector<string> details;
  std::chrono::system_clock::time_point createdAt_;
  uint amount_;
  moneyTxType type_;
  std::string from_account;
  std::string to_account;
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
