
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "account.h"
#include "bank.h"
#include "stock/server.h"

int main() {
  auto &server = stock::server::getInstance();
  std::thread stockUpdaterThread([&]() { server.startUpdateStocksWorker(); });
  std::thread stockOrderThread([&]() { server.startStockWorker(); });

  Bank danskeBank;
  Bank nordea;
  Bank jyskeBank;
  auto &currentBank = danskeBank;

  std::string accId = "12345";
  currentBank.addAccount("Jens Nielsen", accId);
  auto &acc = currentBank.getAccountById(accId);
  bool run = true;

  while (run) {
    int input;
    std::cin >> input;
    std::cout << "1: withdraw money " << std::endl;
    std::cout << "2: deposit money " << std::endl;
    std::cout << "3: buy stock" << std::endl;
    std::cout << "4: sell stock" << std::endl;
    std::cout << "5: switch bank" << std::endl;
    std::cout << "6: monitor stocks" << std::endl;
    std::cout << "7: " << std::endl;
    std::cout << "8: " << std::endl;
    std::cout << "9: " << std::endl;
    std::cout << "e: exit program" << std::endl;

    switch (input) {
    case '1':
      std::cout << "Enter amount to withdraw" << std::endl;
      int withdrawAmount;
      std::cin >> withdrawAmount;
      try {
        acc.withdraw(withdrawAmount);
      } catch (const std::invalid_argument e) {
        std::cout << "Failed to withdraw because: " << e.what() << std::endl;
      }
      std::cout << "Successfull withdrawal!" << std::endl;

    case '2':
      std::cout << "Enter amount to deposit" << std::endl;
      int depositAmount;
      std::cin >> depositAmount;
      acc.withdraw(depositAmount);
      std::cout << "Successfull deposit!" << std::endl;

    case '3':
      std::cout << "Enter number of bank to switch to" << std::endl;
      std::cout << "Available banks:" << std::endl;
      std::cout << "1 : Dansk Bank" << std::endl;
      std::cout << "2 : Nordea" << std::endl;
      std::cout << "3 : JyskeBank" << std::endl;
      int bankNo;
      std::cin >> bankNo;
      if (bankNo == 1 && currentBank != danskeBank) {
        danskeBank.switchToThisBank(currentBank, acc.getId());
        currentBank = danskeBank;
      }
      if (bankNo == 1 && currentBank != danskeBank) {
        danskeBank.switchToThisBank(currentBank, acc.getId());
        currentBank = danskeBank;
      }
      if (bankNo == 1 && currentBank != danskeBank) {
        danskeBank.switchToThisBank(currentBank, acc.getId());
        currentBank = danskeBank;
      }
    case '4':
      std::cout << "4" << std::endl;
    case '5':
      std::cout << "5" << std::endl;
    case '6':
      std::cout << "6" << std::endl;
    case '7':
      std::cout << "7" << std::endl;
    case '8':
      std::cout << "8" << std::endl;
    case '9':
      std::cout << "9" << std::endl;
    case 'e': {
      server.stopWorkers();
      run = false;
    }
    default:
      std::cout << "Unknown input" << std::endl;
    }
  }
  stockUpdaterThread.join();
  stockOrderThread.join();
}
