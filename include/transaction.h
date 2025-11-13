//
// Created by ali on 10/30/25.
//

#ifndef BANK_TRANSACTION_H
#define BANK_TRANSACTION_H
#include <chrono>


class moneyTransaction {
public:
    moneyTransaction(int amount);
    std::chrono::system_clock::time_point getCreatedAt() const;
    int getAmount() const;
private:
    std::chrono::system_clock::time_point _createdAt;
    int32_t _amount;
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