#include <iostream>
#include <memory>
#include "account.hpp"
#include "savingsAccount.hpp"
#include "currencyExchange.hpp"
#include "log.hpp"
#include "transaction.hpp"

using namespace std;

int main() {
    try {

        if (Account::canCreateAccount()) {
            auto acc1 = make_unique<SavingsAccount>("Avantika", 1000);
            Account::addAccount(std::move(acc1));
        }

        if (Account::canCreateAccount()) {
            auto acc2 = make_unique<SavingsAccount>("Avantika", 500);
            Account::addAccount(std::move(acc2));
        }

        cout << "Total accounts: " << Account::getAccountCount() << "\n";

        auto accounts = Account::getAllAccounts();
        accounts[0]->deposit(200);
        accounts[0]->withdraw(100);

        cout << "Balance in first account: " << accounts[0]->getBalance() << "\n";


        currencyExchange ex("USD", "DKK", 7.0);
        double converted = ex.convert(100);
        cout << "Converted 100 USD to " << converted << " DKK\n";

        transaction tx(150, "Deposit");
        cout << tx << "\n";

        cout << "\n--- Currency Exchange Log ---\n";
        ex.showLog();

        cout << "\n--- Account Logs ---\n";

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
