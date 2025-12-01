#include "transaction.hpp"

transaction::transaction(double amt, const string& t)
    : amount(amt), type(t), timestamp(std::chrono::system_clock::now()),
      details(std::pmr::polymorphic_allocator<string>(memRes)) {
}

double transaction::getAmount() const {
    return amount;
}

string transaction::getType() const {
    return type;
}

std::chrono::system_clock::time_point transaction::getTimestamp() const {
    return timestamp;
}

