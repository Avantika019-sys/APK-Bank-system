#include "Tx.h"
#include "TxDetails.h"
#include "util/Logger.h"
#include <string>
#include <sys/types.h>
#include <vector>
#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H
namespace bank {
class Account {
public:
  Account(util::Logger *logger);

  void deposit(int amount);

  void withdraw(int amount);

  void printTransactionHistory() const;

  int getBalance() const;
  void addTransaction(details &&txDetail);

  Account(Account &&other) noexcept;
  Account &operator=(Account &&other) noexcept;

private:
  std::pmr::vector<Tx> txs_{&pool_};
  std::pmr::monotonic_buffer_resource pool_;
  util::Logger *logger_;
  std::mutex mtx_;
};
} // namespace bank
#endif // BANK_ACCOUNT_H
