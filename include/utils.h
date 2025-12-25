#ifndef BANK_UTILS_H
#define BANK_UTILS_H
#include <future>
#include <iostream>
#include <string>
constexpr int operator"" _DKK(long double amount);

template <typename T> void spin(std::future<T> &fut, std::string msg) {
  char spinner[] = {'/', '-', '\\', '|'};
  int idx = 0;
  while (fut.wait_for(std::chrono::milliseconds(100)) !=
         std::future_status::ready) {
    std::cout << "\r[" << spinner[idx % 4] << "] " << msg << std::flush;
    idx++;
  }
}
#endif // BANK_UTILS_H
