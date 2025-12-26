

#include "logger.h"
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
