//
// Created by ali on 10/30/25.
//

#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H
#include <vector>

#include "transaction.h"

#endif //BANK_ACCOUNT_H

template <typename T>
class Account {
    public:
    bool deposit(int amount) {/*implement me*/}
    bool withdraw(int amount) {/*implement me*/}


private:
std::vector<transaction> _transactions;
};