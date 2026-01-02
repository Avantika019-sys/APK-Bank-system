#include "Miner.h"
#include "Server.h"
#include "asset/Crypto.h"
#include "exchange/Manager.h"
#include "exchange/dashboard.h"
#include "exchange/util.hpp"
#include "message.hpp"
#include <boost/smart_ptr/make_shared_array.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>

using namespace std::chrono_literals;
using namespace exchange;
using namespace exchange::currency;
using namespace exchange::util;
using namespace exchange::asset;
using namespace std::placeholders;

int main() {
  auto stockServ = Server<Stock>(std::make_unique<util::Logger>("stockServer"));
  stockServ.addAsset("APPL", Stock("Apple", {1.23_K, 1.235_K}));
  stockServ.addAsset("TSLA", Stock("Tesla technologies", {2.322_K, 2.529_K}));
  stockServ.addAsset("NVDA", Stock("Nvidia", {2.34234_K, 2.34241_K}));

  auto cryptoServ =
      Server<Crypto>(std::make_unique<util::Logger>("cryptoServer"));
  cryptoServ.addAsset("BTC", Crypto("Bitcoin", {0.532_Million, 0.530_Million}));
  cryptoServ.addAsset("ETH", Crypto("Etherium", {32.423_K, 32.451_K}));
  cryptoServ.addAsset("SOL", Crypto("Solana", {23.423_K, 23.451_K}));
  Miner miner("BTC", cryptoServ);

  auto acc = boost::make_shared<Account>();
  auto cryptoMgr = createManager<Crypto>(cryptoServ, acc, "456");
  auto stockMgr = createManager<Stock>(stockServ, acc, "123");

  std::cout << "Subscribing to BTC price updates" << std::endl;
  auto conn = cryptoServ.subscribeToPriceUpdates("BTC", printDatapoint);
  std::this_thread::sleep_for(5s);
  std::cout << "Unsubscribing... bye!" << std::endl;
  conn.disconnect();

  acc->deposit(50.0_K);
  try {
    cryptoMgr->purchaseAsset("SOL", DKK(1.0_K));
    stockMgr->purchaseAsset("APPL", DKK(0.5_K));

    cryptoMgr->addStopLossRule("SOL", DKK(0.532_K));
    stockMgr->sellAsset("APPL", DKK(273));
  } catch (std::invalid_argument e) {
    std::cout << "Error: " << e.what() << std::endl;
  }
  acc->printBalance();
  acc->printTransactionHistory();
  cryptoMgr->printPortfolioStats();
  stockMgr->printPortfolioStats();
}
