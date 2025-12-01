#include <string>
using namespace std;

int Account::accountCount = 0;

class Account {
    private:
        double balance;
        string accountHolderName;
        string accountType;
        static int accountCount;
        static const int maxAccounts = 5;
    public: 
        Account(string holderName, string type, double initialBalance)
            : accountHolderName(holderName), accountType(type), balance(initialBalance) {}
        ~Account() {}
        static int getAccountCount() {}
        virtual double getBalance() const = 0;
        virtual std::string getAccountType() const = 0;
        virtual std::string getAccountHolderName() const = 0;
};

// 1. this class lets the user to create 1 up to 5 accounts which is the limit 