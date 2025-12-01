#include "account.h"
#include "transaction.h"
#include <iostream>
#include <numeric>
#include <string>

Account::Account(std::string name, std::string id) {
  name_ = name;
  id_ = id;
  const std::string fileName = id_ + ".csv";
  fptrLogs_ = std::fopen(fileName.c_str(), "w");
  if (fptrLogs_ == NULL) {
    throw std::invalid_argument("Could not open log file");
  }
}

Account::Account(Account &&other) noexcept
  : moneyTxs_(std::move(other.moneyTxs_)), name_(std::move(other.name_)),
    fptrLogs_(other.fptrLogs_) {
  other.fptrLogs_ = nullptr;
}

Account &Account::operator=(Account &&other) noexcept {
  if (this != &other) {
    if (fptrLogs_ != nullptr) {
      std::fclose(fptrLogs_);
    }

    moneyTxs_ = std::move(other.moneyTxs_);
    fptrLogs_ = other.fptrLogs_;
    name_ = std::move(other.name_);
    other.fptrLogs_ = nullptr;
  }
  log.addEntry("Account created for " + holderName + " with initial balance " + to_string(initialBalance));
  return *this;

}

Account::~Account() {
  if (fptrLogs_ != nullptr) {
    fclose(fptrLogs_);
  }
}

void Account::deposit(int amount) {
  moneyTx  tx(amount,moneyTxType::deposit);
  moneyTxs_.push_back(tx);
  char logMsg[50] = "deposited money into account\n";
  fputs(logMsg, fptrLogs_);
}

void Account::withdraw(int amount) {
  int curBalance = getCurrentBalance();
  if (curBalance < amount) {
    char logMsg[50] = "withdraw attempt failed due to low balance\n";
    fputs(logMsg, fptrLogs_);
    throw std::invalid_argument("Not enough money on account");
  }
  moneyTx tx(amount, moneyTxType::withdraw);
  moneyTxs_.push_back(tx);
}

<<<<<<< HEAD
void Account::printTransactionHistory() const{
  std::for_each(moneyTxs_.begin(), moneyTxs_.end(),
                [](const moneyTx &tx) { std::cout << tx << std::endl; });
}

int Account::getCurrentBalance() const {
  int res = std::accumulate(moneyTxs_.begin(), moneyTxs_.end(), 0,
                            [](int acc, const moneyTx &tx) {
                              switch (tx.getTransactionType()) {
                                case moneyTxType::deposit:
                                  return acc + tx.getAmount();
                                case moneyTxType::withdraw:
                                  return acc - tx.getAmount();
                                case moneyTxType::stockPurchase:
                                  return acc - tx.getAmount();
                              };
                            });
  return res;
}

std::string Account::getId() const{ return id_; }
