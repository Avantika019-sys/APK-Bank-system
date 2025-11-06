//
// Created by ali on 10/30/25.
//

#ifndef BANK_TRANSACTION_H
#define BANK_TRANSACTION_H
#include <chrono>


class transaction {
public:
    transaction(int amount);
    std::chrono::system_clock::time_point getCreatedAt() const;
    int getAmount() const;
private:
    std::chrono::system_clock::time_point _createdAt;
    int32_t _amount;
};


#endif //BANK_TRANSACTION_H