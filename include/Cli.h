

#include "bank.h"
class Cli {
public:
  Cli();
  void loop();

private:
  void handleWithdraw();
  void handleDeposit();
  void handleBuyStock();
  void handleSellStock();
  void handleSwitchBank();
  bool run = true;
  const Bank *currentBank;
  StockAccount *acc;
  std::string accId = "12345";
};
