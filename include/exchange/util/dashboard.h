

#include "exchange/currency/DKK.h"
#include <iostream>
#include <string>
#ifndef EXCHANGE_UTIL_DASHBOARD_H
#define EXCHANGE_UTIL_DASHBOARD_H
namespace exchange::util {
inline void printDatapoint(currency::DKK amnt) {
  std::cout << "Price change: " << amnt << std::endl;
}
} // namespace exchange::util

#endif // EXCHANGE_UTIL_DASHBOARD_H
