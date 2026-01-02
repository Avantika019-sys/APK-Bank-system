
#include <chrono>
#include <future>
#include <iostream>
#include <vector>
#ifndef EXCHANGE_UTIL_SPINNER_H
#define EXCHANGE_UTIL_SPINNER_H
using namespace std::chrono_literals;
namespace exchange::util {
inline void spin(std::string msg) {
  std::vector<char> chars{'-', '\\', '|', '/'};
  static int i = 0;
  std::cout << "\r" << chars[i] << " " << msg << std::flush;
  i++;
  i = i % 4;
}
} // namespace exchange::util
#endif // EXCHANGE_UTIL_SPINNER_H
