

#include "Bank.h"
#include <string>

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
  void handleAddStopLossRule();
  void handleRemoveStopLossRule();
  bool run = true;
  const Bank *currentBank;
  bank::stock::StockAccount *acc;
  std::string accId = "12345";
};
