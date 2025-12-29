
#include "asset/Server.h"
#include "asset/bots/Crypto.h"
#include "asset/messages/Info.h"
#include "asset/types/Crypto.h"
#include "bank/Bank.h"
#include "bank/User.h"
#include <stdexcept>

int main() {
  asset::Server<asset::types::Stock> stockServ;
  stockServ.addAsset("APPL", asset::types::Stock{"Apple", {1231}});
  stockServ.addAsset("TSLA", asset::types::Stock{"Tesla technologies", {2322}});
  stockServ.addAsset("MSFT", asset::types::Stock{"Microsoft", {2342}});
  stockServ.addAsset("NVDA", asset::types::Stock{"Nvidia", {234234}});
  stockServ.pushMsg(asset::messages::OrderEvent<asset::types::Stock>{
      asset::CalculateDemandStatistics<asset::types::Stock>});

  asset::Server<asset::types::Crypto> cryptoServ;
  cryptoServ.addAsset("BTC", asset::types::Crypto{"Bitcoin", {32423}});
  cryptoServ.addAsset("ETH", asset::types::Crypto{"Etherium", {32423}});
  cryptoServ.addAsset("DOGE", asset::types::Crypto{"Doge coin", {324}});
  cryptoServ.addAsset("SOL", asset::types::Crypto{"Solana", {23423}});
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

  b1.addUser("Jens Thomasen", "3535134365");
  b1.addUser("Thomas Simonsen", "9235638135", &cryptoServ);
  b2.addUser("Magnus Jensen", "8139638135", &stockServ);
  b2.addUser("Simon Jensen", "1235638135", &cryptoServ, &stockServ);

  {
    auto &user = b2.getUserByCpr("1235638135");
    user.account->deposit(50000);
    user.account->deposit(50000);
    user.account->deposit(50000);
    user.account->printTransactionHistory();
    // try {
    //   user.cryptoManager->buyAsset("BTC", 1);
    //   user.stockManager->buyAsset("APPL", 5);
    //
    //   user.cryptoManager->addStopLossRule("BTC", 32000);
    //   user.stockManager->sellAsset("APPL", 3);
    // } catch (std::invalid_argument e) {
    //   std::cout << "Error: " << e.what() << std::endl;
    // }
    // user.account->getBalance();
    // user.cryptoManager->printPortfolio();
    // user.stockManager->printPortfolio();
  }
}
