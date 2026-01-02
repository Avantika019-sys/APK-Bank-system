

#include "exchange/currency/DKK.h"
#include <iostream>
#include <string>
#ifndef EXCHANGE_DASHBOARD_H
#define EXCHANGE_DASHBOARD_H
namespace exchange {
inline void printDatapoint(std::string symbol, currency::DKK amnt) {
  std::cout << "Price change: " << amnt << std::endl;
}
} // namespace exchange

#endif // EXCHANGE_DASHBOARD_H
