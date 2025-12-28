#include "TxDetails.h"
#include <chrono>
#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>

#ifndef BANK_TX_H
#define BANK_TX_H
using namespace std;
namespace bank {
class Tx {

public:
  Tx(details d, std::pmr::memory_resource *memRes);
  const details &getDetails() const;
  std::chrono::system_clock::time_point getCreatedAt() const;

private:
  std::pmr::memory_resource *memRes_;
  std::chrono::system_clock::time_point createdAt_;
  details details_;
};
std::ostream &operator<<(std::ostream &os, const Tx &t);
} // namespace bank
#endif // BANK_TX_H
