#include "account.hpp"
#include <iostream>
#include <string>
using namespace std;

class SavingsAccount : public Account {
public:
    SavingsAccount(string holderName, double initialBalance)
        : Account(holderName, "Savings", initialBalance) {}

    double getBalance() const override { return Account::getBalance(); }
    string getAccountType() const override { return Account::getAccountType(); }
    string getAccountHolderName() const override { return Account::getAccountHolderName(); }

    void deposit(double amount) override {
        Account::deposit(amount);
        cout << "Deposited " << amount << " into Savings Account\n";
    }

    void withdraw(double amount) override {
        Account::withdraw(amount);
        cout << "Withdrew " << amount << " from Savings Account\n";
    }
};