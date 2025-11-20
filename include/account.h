//
// Created by ali on 10/30/25.
//

#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H
#include <iostream>
#include <numeric>
#include <vector>

#include "stock/server.h"
#include "transaction.h"

#endif // BANK_ACCOUNT_H

class Account {
public:
  Account(int initialSize, int growthSize)
      : occupiedSize_(0), currentSize_(initialSize), growthSize_(growthSize) {
    transactions_ = new moneyTransaction[currentSize_];
  }

  Account(Account &&other) noexcept
      : transactions_{std::move(other.transactions_)},
        occupiedSize_(other.occupiedSize_), growthSize_(other.growthSize_),
        currentSize_(other.currentSize_) {
    other.transactions_ = nullptr;
    other.occupiedSize_ = 0;
    other.growthSize_ = 0;
    other.currentSize_ = 0;
  }

  ~Account() { delete[] transactions_; }

  void deposit(int amount) {
    if (occupiedSize_ == currentSize_) {
      T *newTransactions = new T[currentSize_ + growthSize_];
      for (int i = 0; i < occupiedSize_; i++) {
        newTransactions[i] = transactions_[i];
      }
      currentSize_ += growthSize_;
      delete[] transactions_;
      transactions_ = newTransactions;
    }
    transactions_[currentSize_++] =
        new moneyTransaction(amount, moneyTransactionType::deposit);
  }

  void withdraw(int amount) {
    int currentBalance =
        std::accumulate(begin(), end(), 0, [](int currentTotal, const T &t) {
          return currentTotal + t.getAmount();
        });
    if (currentBalance < amount) {
      throw std::invalid_argument("not enough money to withdraw that amount");
    }
    if (occupiedSize_ == currentSize_) {
      moneyTransaction *newTransactions =
          new moneyTransaction[currentSize_ + growthSize_];
      for (int i = 0; i < occupiedSize_; i++) {
        newTransactions[i] = transactions_[i];
      }
      currentSize_ += growthSize_;
      delete[] transactions_;
      transactions_ = newTransactions;
    }
    transactions_[currentSize_++] =
        new moneyTransaction(amount, moneyTransactionType::withdrawal);
  }

  void printTransactionHistory() {
    std::for_each(begin(), end(), [](const moneyTransaction &t) {
      std::cout << t << std::endl;
    });
  }

  int getCurrentBalance() {
    int currentBalance = std::accumulate(
        begin(), end(), 0, [](int currentTotal, const moneyTransaction &t) {
          return currentTotal + t.getAmount();
        });
    return currentBalance;
  }

  moneyTransaction *begin() { return transactions_; }

  moneyTransaction *end() { return transactions_ + currentSize_; }

private:
  Account(const Account &other) {}

  Account &operator=(const Account &other) {}

  Account &operator=(Account &&other) noexcept {}

  moneyTransaction *transactions_;
  int occupiedSize_;
  int currentSize_;
  int growthSize_;
};

template <> class Account<stockTransaction> {
public:
  Account() { stock::server::sig.connect(onStockUpdate); }

  void buyStock(u_int32_t amount, std::string stockName) {
    // implement validation: check if there is enough money on account before

    stockTransaction tx(amount, stockName);

    stock::order order;
    order.tx = tx;

    std::future<bool> f = order.prom.get_future();

    stock::server::getInstance().msgQueue.push(order);
    f.wait();
    std::lock_guard<std::mutex> lock(mtx);
    if (f.get()) {
      if (ownedStocks.contains(stockName)) {
        ownedStocks[stockName] += amount;
      } else {
        ownedStocks[stockName] = amount;
      }
    }
  }

  void sellStock(u_int32_t amount, std::string name) {}

  void onStockUpdate(std::string stockName, int updatedPrice) {
    std::lock_guard<std::mutex> lock(mtx);
    if (ownedStocks.contains(stockName)) {
      ownedStocks[stockName] = updatedPrice;
    }
  }

private:
  std::vector<moneyTransaction> _moneyTransactions;
  std::map<std::string, int> ownedStocks;
  std::mutex mtx;
};
