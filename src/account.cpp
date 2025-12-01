#include "account.hpp"
#include <string>
#include <stdexcept>
using namespace std;

Account::Account(string holderName, string type, double initialBalance)
    : accountHolderName(holderName), accountType(type), balance(initialBalance) {
    if (accountCount <= maxAccounts) {
        accountCount++;
    } else {
        throw std::runtime_error("Maximum number of accounts reached.");
    }
}
Account::~Account() {
    accountCount--;
}

int Account::getAccountCount() {
    return accountCount;
}

double Account::getBalance() const {
    return balance;
}
string Account::getAccountType() const {
    return accountType;
}
string Account::getAccountHolderName() const {
    return accountHolderName;
}