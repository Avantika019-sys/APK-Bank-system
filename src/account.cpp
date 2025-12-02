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
    log.addEntry("Account created for " + holderName + " with initial balance " + to_string(initialBalance));
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

void Account::addAccount(unique_ptr<Account> account) {
    if (canCreateAccount()) {
        accounts.push_back(std::move(account));
    } else {
        throw std::runtime_error("Cannot create more accounts. Maximum limit reached.");
    }
}

vector<Account*> Account::getAllAccounts() {
    vector<Account*> accountPointers;
    for (const auto& acc : accounts) {
        accountPointers.push_back(acc.get());
    }
    return accountPointers;
}

void Account::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
    } else {
        throw std::runtime_error("Deposit amount must be positive.");
    }
}

void Account::withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
    } else {
        throw std::runtime_error("Invalid withdrawal amount.");
    }
}


//concepts used
