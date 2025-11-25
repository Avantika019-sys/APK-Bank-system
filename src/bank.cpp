
#include "bank.h"

Bank::Bank(std::string bankName) {
}

void Bank::transferAccountTo(Bank &from, std::string accId) {
    auto it = std::find_if(
        from.accounts.begin(),
        from.accounts.end(),
        [&](const Account &acc) {
            return acc.getId() == accId;
        });
    if (it != from.accounts.end()) {
        accounts.push_back(std::move(*it));
        from.accounts.erase(it);
    }
}



