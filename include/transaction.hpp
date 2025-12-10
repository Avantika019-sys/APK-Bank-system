#include <string>
#include <memory_resource>
#include <iostream>
#include <vector>
#include <chrono>
#include "transaction.h"
using namespace std;

enum class moneyTxType {
  deposit,
  withdraw,
  stockPurchase,
};

// enum stock, buy sell

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
 std::string from_account;
    std::string to_account;

    public:
        moneyTxType getTransactionType() const;
        moneyTx(double amt, const string& t)
            : amount(amt), type(t), details(std::pmr::polymorphic_allocator<string>(memRes)) {
        }
        int getAmount() const {}
        string getType() const {}
        std::chrono::system_clock::time_point getCreatedAt() const;
}; 



class stockTx {
public:
  stockTx();

  stockTx(int amount, std::string stockName);

  std::chrono::system_clock::time_point getCreatedAt() const;

  int getAmount() const;

private:
  std::chrono::system_clock::time_point _createdAt;
  int amount_;
};

std::ostream &operator<<(std::ostream &os, const moneyTx &t);
std::ostream &operator<<(std::ostream &os, const stockTx &t);


