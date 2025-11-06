//
// Created by ali on 10/30/25.
//

#include "../include/transaction.h"

transaction::transaction(const int amount): _createdAt(std::chrono::system_clock::now()),_amount(amount) {
}

std::chrono::system_clock::time_point transaction::getCreatedAt() const {
    return _createdAt;
}

int transaction::getAmount() const {
    return _amount;
}

std::ostream& operator<<(std::ostream& os, const transaction& t) {
    os << "Transaction created at: " << t.getCreatedAt() << "Amount: "<< t.getAmount() << std::endl;
}