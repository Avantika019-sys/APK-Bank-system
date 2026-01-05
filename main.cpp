#include "Miner.h"
#include "Server.h"
#include "asset/Crypto.h"
#include "exchange/Manager.h"
#include "exchange/asset.hpp"
#include "exchange/currency/DKK.h"
#include "exchange/util.hpp"
#include "exchange/util/Literals.h"
#include "exchange/util/dashboard.h"
#include "message.hpp"
#include <algorithm>
#include <boost/smart_ptr/make_shared_array.hpp>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace std::chrono_literals;
using namespace exchange;
using namespace exchange::currency;
using namespace exchange::util;
using namespace exchange::asset;
using namespace std::placeholders;

int main() {
  MonitorResource cryptoMs;
  Crypto c1("Bitcoin", "BTC", &cryptoMs);
  Crypto c2("Etherium", "ETH", &cryptoMs);
  c1.unitPriceOverTime_.emplace_back(0.531_Mil);
  c1.unitPriceOverTime_.emplace_back(1.25_K);
  c2.unitPriceOverTime_.emplace_back(10.2_K);
  c2.unitPriceOverTime_.emplace_back(13.2_K);
  auto cryptoServ = createServer<Crypto>("cryptoServer.txt", &cryptoMs);
  cryptoServ->addAsset("BTC", std::move(c1));
  cryptoServ->addAsset("ETH", std::move(c2));
  cryptoServ->start();
  Miner miner("BTC", cryptoServ);

  MonitorResource stockMs;
  Stock s1("Apple", "APPL", &stockMs);
  Stock s2("Tesla motor technologies", "TSLA", &stockMs);
  s1.unitPriceOverTime_.emplace_back(1.23_K);
  s1.unitPriceOverTime_.emplace_back(1.25_K);
  s2.unitPriceOverTime_.emplace_back(10.2_K);
  s2.unitPriceOverTime_.emplace_back(13.2_K);
  auto stockServ = createServer<Stock>("stockServer.txt", &stockMs);
  stockServ->addAsset("APPL", std::move(s1));
  stockServ->addAsset("TSLA", std::move(s2));
  stockServ->start();
  auto acc = boost::make_shared<Account>();
  auto cryptoMgr = Manager<Crypto>(cryptoServ, acc, "456");
  auto stockMgr = Manager<Stock>(stockServ, acc, "123");

  std::cout << "Subscribing to BTC price updates" << std::endl;
  auto conn = cryptoServ->subscribeToPriceUpdates("BTC", printDatapoint);
  std::this_thread::sleep_for(5s);
  std::cout << "Unsubscribing... bye!" << std::endl;
  conn.disconnect();

  acc->deposit(50.0_K);
  cryptoMgr.purchaseAsset("ETH", DKK(1.0_K));
  stockMgr.purchaseAsset("APPL", DKK(0.5_K));

  stockMgr.addStopLossRule("APPL", DKK(1.0_K));
  stockMgr.sellAsset("APPL", DKK(273));
  acc->printBalance();
  acc->printTransactionHistory();
  cryptoMgr.printPortfolioStats();
  stockMgr.printPortfolioStats();
}
