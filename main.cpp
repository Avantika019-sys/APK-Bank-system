
#include "asset/Server.h"
#include "asset/types/Crypto.h"
#include "bank/Bank.h"
#include "bank/User.h"

int main() {
  asset::Server<asset::types::Stock> stockServ;
  stockServ.addAsset("APPL", asset::types::Stock("Apple"));
  stockServ.addAsset("TSLA", asset::types::Stock("Tesla motor technologies"));
  stockServ.addAsset("MSFT", asset::types::Stock("Microsoft"));
  stockServ.addAsset("NVDA", asset::types::Stock("Nvidia"));

  asset::Server<asset::types::Crypto> cryptoServ;
  cryptoServ.addAsset("BTC", asset::types::Crypto("Bitcoin"));
  cryptoServ.addAsset("ETH", asset::types::Crypto("Etherium"));
  cryptoServ.addAsset("DOGE", asset::types::Crypto("Doge coin"));
  cryptoServ.addAsset("SOL", asset::types::Crypto("Solana"));

  bank::Bank b1;
  bank::Bank b2;

  b1.addUser("Jens Thomasen", "3535134365");
  b1.addUser("Thomas Simonsen", "9235638135", &cryptoServ);
  b2.addUser("Simon Jensen", "1235638135", &cryptoServ, &stockServ);

  {
    auto &user = b2.getUserByCpr("1235638135");
    user.cryptoManager->buyAsset("BTC", 10);
    user.cryptoManager->printPortfolio();
  }
}
