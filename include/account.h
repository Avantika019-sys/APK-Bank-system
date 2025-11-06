//
// Created by ali on 10/30/25.
//

#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H
#include <iostream>
#include <numeric>
#include <vector>

#include "transaction.h"

#endif //BANK_ACCOUNT_H

template <typename T>
class Account {
    public:
    void deposit(u_int32_t amount) {
        std::vector<moneyTransaction> tmp(_transactions);
        moneyTransaction newTransaction(-amount);
        tmp.push_back(newTransaction);
        std::swap(_transactions,tmp);
    }
    void withdraw(u_int32_t amount) {
        int currentBalance = std::accumulate(_transactions.begin(),_transactions.end(),0,
            [](int currentTotal,const T& t) {
                return currentTotal + t.getAmount();
            });

        if (currentBalance < amount) {
           throw std::invalid_argument("not enough money to withdraw that amount");
        }
        std::vector<moneyTransaction> tmp(_transactions);
        moneyTransaction newTransaction(amount);
        tmp.push_back(newTransaction);
        std::swap(_transactions,tmp);
    }
    void viewTransactionHistory() {
        std::for_each(_transactions.begin(),_transactions.end(),[](const moneyTransaction& t) {
            std::cout << t << std::endl;
        });
    }
    void checkCurrentBalance() {
        int currentBalance = std::accumulate(_transactions.begin(),_transactions.end(),0,
            [](int currentTotal,const moneyTransaction& t) {
                return currentTotal + t.getAmount();
            });
        std::cout << "your current balance is: "<< currentBalance << std::endl;
    }


private:
std::vector<T> _transactions;
};


template<>
class Account<stockTransaction> {
    public:
    void buyStock(u_int32_t amount, std::string name) {

    }
    void sellStock(u_int32_t amount, std::string name) {

    }

private:
    std::vector<moneyTransaction> _moneyTransactions;
};

