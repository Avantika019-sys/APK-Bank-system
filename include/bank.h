//
// Created by ali on 11/20/25.
//

#ifndef BANK_BANK_H
#define BANK_BANK_H
#include <vector>

#include "account.h"

class Bank {
public:
    Bank();

private:
    std::vector<Account> accounts;
};
#endif //BANK_BANK_H
