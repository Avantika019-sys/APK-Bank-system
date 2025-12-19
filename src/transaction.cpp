
#include "transaction.h"
#include <iostream>

Tx::Tx(uint amt, TxType type, std::pmr::memory_resource *memRes)
    : memRes_(memRes), details_(memRes), amount_(amt), type_(type),
      createdAt_(std::chrono::system_clock::now()) {}

std::chrono::system_clock::time_point Tx::getCreatedAt() const {
  return createdAt_;
}

const details &Tx::getDetails() const { return details_; }

std::ostream &operator<<(std::ostream &os, const Tx &t) {
  std::string detailsStr = std::visit(ToString{}, t.getDetails());
  os << "Transaction created at: " << t.getCreatedAt() << detailsStr
     << std::endl;
  return os;
}
