
#include <iostream>
#include <ostream>

#include "stock/server.h"

int main() {
  auto &server = stock::server::getInstance();
  std::thread stockUpdaterThread([&]() { server.startUpdateStocksWorker(); });
  std::thread stockOrderThread([&]() { server.startStockWorker(); });

  int input;

  while (true) {
    std::cin >> input;
    std::cout << "1: withdraw money " << std::endl;
    std::cout << "2: deposit money " << std::endl;
    std::cout << "3: " << std::endl;
    std::cout << "4: " << std::endl;
    std::cout << "5: " << std::endl;
    std::cout << "6: " << std::endl;
    std::cout << "7: " << std::endl;
    std::cout << "8: " << std::endl;
    std::cout << "9: " << std::endl;


    switch (input) {
      case '1': std::cout << "1" << std::endl;
      case '2': std::cout << "2" << std::endl;
      case '3': std::cout << "3" << std::endl;
      case '4': std::cout << "4" << std::endl;
      case '5': std::cout << "5" << std::endl;
      case '6': std::cout << "6" << std::endl;
      case '7': std::cout << "7" << std::endl;
      case '8': std::cout << "8" << std::endl;
      case '9': std::cout << "9" << std::endl;

      default:
        std::cout << "Unknown input" << std::endl;
    }
  }
  stockUpdaterThread.join();
  stockOrderThread.join();
}
