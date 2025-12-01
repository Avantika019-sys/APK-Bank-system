<<<<<<< HEAD
=======
#include <string>
#include <vector>
#include <memory>
#include "log.hpp"
using namespace std;

class Account {
    private:
        double balance;
        string accountHolderName;
        string accountType;
        static int accountCount;
        static const int maxAccounts = 5;
        static vector<unique_ptr<Account>> accounts;
        log log;

    public: 
        Account(string holderName, string type, double initialBalance)
            : accountHolderName(holderName), accountType(type), balance(initialBalance) {
            accountCount++;
        } 
        virtual ~Account() { accountCount--; }
        
        static int getAccountCount() { return accountCount; }
        static bool canCreateAccount() { return accountCount < maxAccounts; }
        static void addAccount(unique_ptr<Account> account);
        static vector<Account*> getAllAccounts();
        
        virtual double getBalance() const = 0;
        virtual std::string getAccountType() const = 0;
        virtual std::string getAccountHolderName() const = 0;
        virtual void deposit(double amount) = 0;
        virtual void withdraw(double amount) = 0;
};

int Account::accountCount = 0;
vector<unique_ptr<Account>> Account::accounts;

//concepts used:    
// - Static members to track account count and store accounts
// - Pure virtual functions to enforce derived class implementations
// - Use of unique_ptr for memory management
// - vector container to hold multiple accounts
>>>>>>> b744aeb (commit)
