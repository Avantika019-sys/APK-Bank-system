
#include "asset/Server.h"
#include "asset/types/Crypto.h"
#include "bank/Bank.h"
#include "bank/User.h"

int main() {
  asset::Server<asset::types::Stock> stockServ;
  asset::Server<asset::types::Crypto> cryptoServ;
  std::thread stockSimThread(
      [&stockServ]() { stockServ.startSimulatingAssetPriceUpdates(); });
  std::thread cryptoSimThread(
      [&cryptoServ]() { cryptoServ.startSimulatingAssetPriceUpdates(); });

  std::thread stockMsgThread(
      [&stockServ]() { stockServ.startMessageProccesor(); });
  std::thread cryptoMsgThread(
      [&cryptoServ]() { cryptoServ.startMessageProccesor(); });

  bank::Bank b1("Nordea");
  bank::Bank b2("Nykredit");

  b1.addUser("Jens Thomasen", "3535134365");
  b1.addUser("Simon Jensen", "1235638135", stockServ, cryptoServ);
  b2.addUser("Simon Jensen", "1235638135", cryptoServ);
}
