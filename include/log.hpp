#include <iostream>
#include <vector>
#include <string>
#include <memory_resource>
using namespace std;

class Log{
    private:
        std::pmr::vector<string> entries;
        std::pmr::memory_resource* memRes = std::pmr::get_default_resource();

    public:
        Log() : entries(std::pmr::polymorphic_allocator<string>(memRes)) {}
        
        void addEntry(const string& entry) {
            entries.push_back(entry);
        }

        void showEntries() const {
            for (const auto& entry : entries) {
                cout << entry << endl;
            }
        }
};
