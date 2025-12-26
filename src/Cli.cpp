#include "Cli.h"
#include "server/MessageQueue.h"
#include "server/messages/Stop.h"
#include "utils.h"
#include <future>
#include <iostream>

Cli::Cli() : serv() {
  // banks_ = {
  //     {"Danske Bank", Bank<Stock>("Danske Bank")},
  //     {"Nordea", Bank<Stock>("Danske Bank")},
  //     {"Jyske Bank", Bank<Stock>("Jyske Bank")},
  // };
  // currentBank = "Dankse Bank";
  // acc = banks_[currentBank].addAssetAccount("Jens Nielsen", accId);
  // acc = bank::account::AssetAccount<Stock>("hej", "h");
}
void Cli::loop() {
  while (run) {
    std::cout << "\n1: Withdraw money " << std::endl;
    std::cout << "2: Deposit money " << std::endl;
    std::cout << "3: Buy stock" << std::endl;
    std::cout << "4: Sell stock" << std::endl;
    std::cout << "5: Switch bank" << std::endl;
    std::cout << "6: " << std::endl;
    std::cout << "7: Get transaction history" << std::endl;
    std::cout << "8: Get current balance" << std::endl;
    std::cout << "9: Get current portfolio value" << std::endl;
    std::cout << "10: Add stop loss rule to portfolio" << std::endl;
    std::cout << "11: Remove stop loss rule from portfolio" << std::endl;
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
      break;
    case 7:
      acc.printTransactionHistory();
      break;
    case 8:
      std::cout << "Current balance: " << acc.getBalance() << std::endl;
      break;
    case 9:
      acc.printPortfolio();
      break;
    case 10:
      handleAddStopLossRule();
      break;
    case 11:
      handleRemoveStopLossRule();
      break;
    case -1:
      // bank::server::Server<Stock>::getInstance().pushMsg(
      //     bank::server::messages::Stop());
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
    acc.withdraw(withdrawAmount);
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
  acc.deposit(depositAmount);
  std::cout << "Successfull deposit!" << std::endl;
}
void Cli::handleBuyStock() {
  std::cout << "AAPL\nMSFT\nGOOG\nTSLA\n";
  std::cout << "Enter stock name to buy: ";
  std::string stockName;
  std::cin >> stockName;
  std::cout << "Enter quantity of the stock to buy: ";
  int qty;
  std::cin >> qty;
  auto fut =
      std::async(std::launch::async, [&]() { acc.buyStock(stockName, qty); });
  spin(fut, "wait for server to proccesing order");
  try {
    fut.get();
  } catch (const std::invalid_argument e) {
    std::cout << "failed to purchase stock: " << e.what() << std::endl;
    return;
  }
  std::cout << "succesfull stock purchase!" << std::endl;
}
void Cli::handleSwitchBank() {
  // std::cout << "Enter name of bank to switch to" << std::endl;
  // std::cout << "Available banks:" << std::endl;
  // std::cout << "1: Dansk Bank" << std::endl;
  // std::cout << "2: Nordea" << std::endl;
  // std::cout << "3: JyskeBank" << std::endl;
  // std::string bankName;
  // std::cin >> bankName;
  // if (!banks_.contains(bankName)) {
  //   std::cout << "Bank with that name does not exist" << std::endl;
  //   return;
  // }
  // try {
  //   banks_[bankName].switchToThisBank(&banks_[currentBank], accId);
  // } catch (const std::invalid_argument e) {
  //   std::cout << " error: already on this bank " << e.what() << std::endl;
  //   return;
  // }
  // std::cout << "Switched bank!" << std::endl;
}
void Cli::handleAddStopLossRule() {}
void Cli::handleSellStock() {}
void Cli::handleRemoveStopLossRule() {}
