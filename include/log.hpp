#include <iostream>
#include <vector>
#include <string>
#include <memory_resource>
using namespace std;

<<<<<<< HEAD
class Log{
=======
class log{
>>>>>>> b744aeb (commit)
    private:
        std::pmr::vector<string> entries;
        std::pmr::memory_resource* memRes = std::pmr::get_default_resource();

    public:
<<<<<<< HEAD
        Log() : entries(std::pmr::polymorphic_allocator<string>(memRes)) {}
=======
        log() : entries(std::pmr::polymorphic_allocator<string>(memRes)) {}
>>>>>>> b744aeb (commit)
        
        void addEntry(const string& entry) {
            entries.push_back(entry);
        }

        void showEntries() const {
            for (const auto& entry : entries) {
                cout << entry << endl;
            }
        }
<<<<<<< HEAD
};
=======

};


// log class where it can show and add entries from the account class
>>>>>>> b744aeb (commit)
