
#include <chrono>
#include <future>
#include <iostream>
#include <vector>
#ifndef BANK_UTILSPINNER_H
#define BANK_UTILSPINNER_H
using namespace std::chrono_literals;
namespace util {
inline void spin(std::string msg) {
  std::vector<char> chars{'-', '\\', '|', '/'};
  int i = 0;
  std::cout << chars[i] << " " << msg << std::flush;
  i++;
  i = i % 4;
}
} // namespace util
#endif // BANK_UTILSPINNER_H
