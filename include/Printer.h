

#include "logger.h"
#include <iostream>
// template <typename T> class PrintTraits {
//   static std::string String() { return ""; };
// };
//
// template <> struct PrintTraits<Tx> {
//   static std::string Header() { return "Transaction:"; };
//   static std::string Footer() { return "Transaction:"; };
// };
//
class Printer {
public:
  template <typename T, typename... Args>
    requires(streamable<T>::value)
  void print(T value, Args... args) {
    std::cout << value << std::endl << std::endl;
    print(args...);
  }
  void print() {}
};
