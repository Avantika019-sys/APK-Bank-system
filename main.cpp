
#include "asset/Server.h"
#include "asset/bots/Crypto.h"
#include "asset/crypto/Miner.h"
#include "asset/messages/Info.h"
#include "asset/types/Crypto.h"
#include "bank/Bank.h"
#include "bank/User.h"
#include "util/Literals.h"
#include <stdexcept>

int main() {
  asset::Server<asset::types::Stock> stockServ;
  stockServ.addAsset("APPL", asset::types::Stock{"Apple", {1.23_K}});
  stockServ.addAsset("TSLA",
                     asset::types::Stock{"Tesla technologies", {2.322_K}});
  stockServ.addAsset("MSFT", asset::types::Stock{"Microsoft", {2.342_K}});
  stockServ.addAsset("NVDA", asset::types::Stock{"Nvidia", {2.34234_K}});
  stockServ.pushMsg(asset::messages::OrderEvent<asset::types::Stock>{
      asset::CalculateDemandStatistics<asset::types::Stock>});

  asset::Server<asset::types::Crypto> cryptoServ;
  cryptoServ.addAsset("BTC", asset::types::Crypto{"Bitcoin", {0.532_Million}});
  cryptoServ.addAsset("ETH", asset::types::Crypto{"Etherium", {32.423_K}});
  cryptoServ.addAsset("DOGE", asset::types::Crypto{"Doge coin", {32.4_K}});
  cryptoServ.addAsset("SOL", asset::types::Crypto{"Solana", {23.423_K}});
  cryptoServ.pushMsg(asset::messages::OrderEvent<asset::types::Crypto>{
      asset::CalculateDemandStatistics<asset::types::Crypto>});
  asset::bots::Crypto bot;
  auto handler = std::bind(&asset::bots::Crypto::OnNewOrder, &bot,
                           std::placeholders::_1, std::placeholders::_2,
                           std::placeholders::_3, std::placeholders::_4,
                           std::placeholders::_5, std::placeholders::_6);
  cryptoServ.pushMsg(
      asset::messages::OrderEvent<asset::types::Crypto>{handler});
  bank::Bank b1;
  bank::Bank b2;
  asset::crypto::Miner miner("BTC", &cryptoServ);
  b1.addUser("Jens Thomasen", "3535134365");
  b1.addUser("Thomas Simonsen", "9235638135", &cryptoServ);
  b2.addUser("Magnus Jensen", "8139638135", &stockServ);
  b2.addUser("Simon Jensen", "1235638135", &cryptoServ, &stockServ);

  {
    auto &user = b2.getUserByCpr("1235638135");
    user.account->deposit(50.0_K);
    try {
      user.cryptoManager->purchaseAsset("SOL", 1);
      user.stockManager->purchaseAsset("APPL", 5);

      user.cryptoManager->addStopLossRule("BTC", 32.0_K);
      user.stockManager->sellAsset("APPL", 3);
    } catch (std::invalid_argument e) {
      std::cout << "Error: " << e.what() << std::endl;
    }
    user.account->getBalance();
    user.account->printTransactionHistory();
    user.cryptoManager->printPortfolio();
    user.stockManager->printPortfolio();
    user.account->generateBankStatement();
  }
}
