
#include "account.h"
#include "bank.h"
#include "stock/server.h"
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

int main() {
  auto &server = stock::server::getInstance();
  std::thread stockUpdaterThread([&]() { server.startUpdateStocksWorker(); });
  std::thread stockOrderThread([&]() { server.startStockWorker(); });

  Bank danskeBank("Danske Bank");
  Bank nordea("Nordea");
  Bank jyskeBank("Jyske Bank");
  Bank *currentBank = &danskeBank;

  std::string accId = "12345";
  currentBank->addStockAccount("Jens Nielsen", accId);
  auto &acc = currentBank->getAccountById(accId);

  bool run = true;
  while (run) {
    std::cout << std::endl;
    std::cout << "1: Withdraw money " << std::endl;
    std::cout << "2: Deposit money " << std::endl;
    std::cout << "3: Buy stock" << std::endl;
    std::cout << "4: Sell stock" << std::endl;
    std::cout << "5: Switch bank" << std::endl;
    std::cout << "6: Monitor stocks" << std::endl;
    std::cout << "7: Get transaction history and current balance" << std::endl;
    std::cout << "8: " << std::endl;
    std::cout << "9: " << std::endl;
    std::cout << "-1: Exit program" << std::endl;
    std::cout << std::endl;

    int input;
    std::cin >> input;
    switch (input) {
    case 1:
      std::cout << "Enter amount to withdraw" << std::endl;
      int withdrawAmount;
      std::cin >> withdrawAmount;
      try {
        acc.withdraw(withdrawAmount);
      } catch (const std::invalid_argument e) {
        std::cout << "Failed to withdraw because: " << e.what() << std::endl;
        continue;
      }
      std::cout << "Successfull withdrawal!" << std::endl;
      break;
    case 2:
      std::cout << "Enter amount to deposit: ";
      int depositAmount;
      std::cin >> depositAmount;
      acc.deposit(depositAmount);
      std::cout << "Successfull deposit!" << std::endl;
      break;
    case 3: {
      std::cout << "Enter stock name to buy: ";
      std::string stockName;
      std::cin >> stockName;
      std::cout << std::endl;
      std::cout << "Enter amount of the stock to buy: ";
      int amount;
      std::cin >> amount;
      acc.buyStock(amount, stockName);
      break;
    }
    case 4:
      std::cout << "4" << std::endl;
      break;
    case 5:
      std::cout << "Enter number of bank to switch to" << std::endl;
      std::cout << "Available banks:" << std::endl;
      std::cout << "1: Dansk Bank" << std::endl;
      std::cout << "2: Nordea" << std::endl;
      std::cout << "3: JyskeBank" << std::endl;
      int bankNo;
      std::cin >> bankNo;
      if (bankNo == 1 &&
          currentBank->getBankName() != danskeBank.getBankName()) {
        danskeBank.switchToThisBank(*currentBank, acc.getId());
        currentBank = &danskeBank;
        std::cout << "Switched to Danske Bank" << std::endl;
      }
      if (bankNo == 2 && currentBank->getBankName() != nordea.getBankName()) {
        danskeBank.switchToThisBank(*currentBank, acc.getId());
        currentBank = &nordea;
        std::cout << "Switched to Nordea" << std::endl;
      }
      if (bankNo == 3 &&
          currentBank->getBankName() != jyskeBank.getBankName()) {
        danskeBank.switchToThisBank(*currentBank, acc.getId());
        currentBank = &jyskeBank;
        std::cout << "Switched to Jyske Bank" << std::endl;
      }
      break;
    case 6:
      std::cout << "6" << std::endl;
      break;
    case 7:
      acc.printTransactionHistory();
      std::cout << "Current balance: " << acc.getBalance() << std::endl;
      break;
    case 8:
      std::cout << "8" << std::endl;
      break;
    case 9:
      std::cout << "9" << std::endl;
      break;
    case -1: {
      server.stopWorkers();
      run = false;
      break;
    }
    default:
      std::cout << "Unknown input" << std::endl;
    }
  }
  stockUpdaterThread.join();
  stockOrderThread.join();
}
