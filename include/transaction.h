
#ifndef BANK_TRANSACTION_H
#define BANK_TRANSACTION_H
#include <chrono>
#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>
using namespace std;

enum class TxType {
  deposit,
  withdraw,
  stockPurchase,
  stockSell,
};

// enum stock, buy sell

class Tx {

public:
  Tx(uint amt, TxType type);
  uint getAmount() const;
  TxType getType() const;
  std::chrono::system_clock::time_point getCreatedAt() const;
  std::string toString();

private:
  std::pmr::memory_resource *memRes = std::pmr::get_default_resource();
  std::pmr::vector<string> details;
  std::chrono::system_clock::time_point createdAt_;
  uint amount_;
  TxType type_;
  std::string from_account;
  std::string to_account;
};

// class stockTx {
// public:
//   stockTx();
//
//   stockTx(int amount, std::string stockName);
//
//   std::chrono::system_clock::time_point getCreatedAt() const;
//
//   int getAmount() const;
//
// private:
//   std::chrono::system_clock::time_point createdAt_;
//   int amount_;
//   std::string stockName_;
// };

std::ostream &operator<<(std::ostream &os, const Tx &t);
// std::ostream &operator<<(std::ostream &os, const stockTx &t);
#endif // BANK_TRANSACTION_H
