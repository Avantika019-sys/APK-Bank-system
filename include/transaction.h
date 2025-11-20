//
// Created by ali on 10/30/25.
//

#ifndef BANK_TRANSACTION_H
#define BANK_TRANSACTION_H
#include <chrono>

enum class moneyTransactionType {
    deposit, withdrawal,
};


class moneyTransaction {
public:
    moneyTransaction(int amount, moneyTransactionType type);

    std::chrono::system_clock::time_point getCreatedAt() const;

    int getAmount() const;

    moneyTransactionType getTransactionType() const;

private:
    std::chrono::system_clock::time_point createdAt_;
    int amount_;
    moneyTransactionType type_;
};

class stockTransaction {
    stockTransaction(int amount, std::string stockName);

    std::chrono::system_clock::time_point getCreatedAt() const;

    int getAmount() const;

private:
    std::chrono::system_clock::time_point _createdAt;
    int32_t _amount;
};

#endif //BANK_TRANSACTION_H
