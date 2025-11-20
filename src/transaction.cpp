//
// Created by ali on 10/30/25.
//

#include "../include/transaction.h"

moneyTransaction::moneyTransaction(const int amount, moneyTransactionType type) : createdAt_(
        std::chrono::system_clock::now()), amount_(amount), type_(type) {
}

std::chrono::system_clock::time_point moneyTransaction::getCreatedAt() const {
    return createdAt_;
}

int moneyTransaction::getAmount() const {
    return amount_;
}

moneyTransactionType moneyTransaction::getTransactionType() const {
    return type_;
}

std::ostream &operator<<(std::ostream &os, const moneyTransaction &t) {
    std::string transactionTypeStr = "withdrawal";
    if (t.getTransactionType() == moneyTransactionType::deposit) {
        transactionTypeStr = "deposit";
    }
    os << "Transaction created at: " << t.getCreatedAt() << " Amount: " << t.getAmount() << " Type: " <<
            transactionTypeStr << std::endl;
}
