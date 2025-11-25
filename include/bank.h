#ifndef BANK_BANK_H
#define BANK_BANK_H
#include <vector>

#include "account.h"

class Bank {
public:
  Bank(std::string bankName);

  void transferAccountTo(Bank &from, std::string accId);

private:
  std::vector<Account> accounts;
};


#endif // BANK_BANK_H
