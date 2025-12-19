#include "Cli.h"
#include "bank.h"

Cli::Cli() {
  Bank danskeBank("Danske Bank");
  Bank nordea("Nordea");
  Bank jyskeBank("Jyske Bank");
  danskeBank.addStockAccount("Jens Nielsen", accId);
  currentBank = &danskeBank;
  auto &acc = currentBank->getAccountById(accId);
}
void Cli::loop() {
  while (run) {
    std::cout << "\n1: Withdraw money " << std::endl;
    std::cout << "2: Deposit money " << std::endl;
    std::cout << "3: Buy stock" << std::endl;
    std::cout << "4: Sell stock" << std::endl;
    std::cout << "5: Switch bank" << std::endl;
    std::cout << "6: Monitor stocks" << std::endl;
    std::cout << "7: Get transaction history" << std::endl;
    std::cout << "8: Get current balance" << std::endl;
    std::cout << "9: " << std::endl;
    std::cout << "-1: Exit program\n" << std::endl;
    int input;
    std::cin >> input;
    switch (input) {
    case 1:
      handleWithdraw();
      break;
    case 2:
      handleDeposit();
      break;
    case 3:
      handleBuyStock();
      break;
    case 4:
      handleSellStock();
      break;
    case 5:
      handleSwitchBank();
      break;
    case 6:
      acc->setMonitorStocks(true);
      break;
    case 7:
      acc->printTransactionHistory();
      break;
    case 8:
      std::cout << "Current balance: " << acc->getBalance() << std::endl;
      break;
    case 9:
      stock::Server::getInstance().stopWorkers();
      run = false;
      break;
    }
  }
}
void Cli::handleWithdraw() {
  std::cout << "Enter amount to withdraw" << std::endl;
  int withdrawAmount;
  std::cin >> withdrawAmount;
  try {
    acc->withdraw(withdrawAmount);
  } catch (const std::invalid_argument e) {
    std::cout << "Failed to withdraw because: " << e.what() << std::endl;
    return;
  }
  std::cout << "Successfull withdrawal!" << std::endl;
}
void Cli::handleDeposit() {
  std::cout << "Enter amount to deposit: ";
  int depositAmount;
  std::cin >> depositAmount;
  acc->deposit(depositAmount);
  std::cout << "Successfull deposit!" << std::endl;
}
void Cli::handleBuyStock() {
  std::cout << "AAPL\nMSFT\nGOOG\nTSLA\n";
  std::cout << "Enter stock name to buy: ";
  std::string stockName;
  std::cin >> stockName;
  std::cout << "Enter amount of the stock to buy: ";
  int amount;
  std::cin >> amount;
  try {
    acc->buyStock(amount, stockName);
  } catch (const std::invalid_argument e) {
    std::cout << "failed to purchase stock: " << e.what() << std::endl;
    return;
  }
  std::cout << "succesfull stock purchase!" << std::endl;
}
void Cli::handleSwitchBank() {
  std::cout << "Enter number of bank to switch to" << std::endl;
  std::cout << "Available banks:" << std::endl;
  std::cout << "1: Dansk Bank" << std::endl;
  std::cout << "2: Nordea" << std::endl;
  std::cout << "3: JyskeBank" << std::endl;
  int bankNo;
  std::cin >> bankNo;
  if (bankNo == 1 && currentBank->getBankName() != danskeBank.getBankName()) {
    danskeBank.switchToThisBank(*currentBank, acc.getId());
    currentBank = &danskeBank;
    std::cout << "Switched to Danske Bank" << std::endl;
  }
  if (bankNo == 2 && currentBank->getBankName() != nordea.getBankName()) {
    danskeBank.switchToThisBank(*currentBank, acc.getId());
    currentBank = &nordea;
    std::cout << "Switched to Nordea" << std::endl;
  }
  if (bankNo == 3 && currentBank->getBankName() != jyskeBank.getBankName()) {
    danskeBank.switchToThisBank(*currentBank, acc.getId());
    currentBank = &jyskeBank;
    std::cout << "Switched to Jyske Bank" << std::endl;
  }
}
