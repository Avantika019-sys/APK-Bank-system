#include <string>
#include <memory_resource>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

class transaction {
    private:
        double amount;
        string type; 
        std::chrono::system_clock::time_point timestamp;
        std::pmr::memory_resource* memRes = std::pmr::get_default_resource();
        std::pmr::vector<string> details;
    public:
        transaction(double amt, const string& t)
            : amount(amt), type(t), details(std::pmr::polymorphic_allocator<string>(memRes)) {
        }
        double getAmount() const {}
        string getType() const {}
        std::chrono::system_clock::time_point getTimestamp() const {}

        friend std::ostream& operator<<(std::ostream& os, const transaction& tx) {
            os << tx.type << " of " << tx.amount;
            return os;
        }
}; 


