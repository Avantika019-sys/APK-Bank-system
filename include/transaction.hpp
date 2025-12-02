#include <string>
#include <memory_resource>
#include <iostream>
#include <vector>
#include <chrono>
#include "transaction.h"
using namespace std;

class moneyTx {
    private:
        double amount;
        string type; 
        std::chrono::system_clock::time_point timestamp;
        std::pmr::memory_resource* memRes = std::pmr::get_default_resource();
        std::pmr::vector<string> details;
        std::chrono::system_clock::time_point createdAt_;
        int amount_;
        moneyTxType type_;


    public:
        moneyTxType getTransactionType() const;
        moneyTx(double amt, const string& t)
            : amount(amt), type(t), details(std::pmr::polymorphic_allocator<string>(memRes)) {
        }
        int getAmount() const {}
        string getType() const {}
        std::chrono::system_clock::time_point getCreatedAt() const;
        std::chrono::system_clock::time_point getTimestamp() const {}

        friend std::ostream& operator<<(std::ostream& os, const moneyTx& tx) {
            os << tx.type << " of " << tx.amount;
            return os;
        }
}; 

std::ostream &operator<<(std::ostream &os, const moneyTx &t);
std::ostream &operator<<(std::ostream &os, const stockTx &t);


