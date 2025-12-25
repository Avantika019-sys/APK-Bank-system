

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
  std::string currentBank;
  bank::account::AssetAccount<Stock> *acc;
  std::string accId = "12345";
  std::map<std::string, Bank<Stock>> banks_;
};
