
#include "./include/asset/Crypto.h"
#include "Bank.h"
#include "Cli.h"
#include "server/Server.h"

int main() {
  auto &serv = bank::server::Server<Crypto>::getInstance();
  std::thread stockUpdaterThread(
      [&]() { serv.startSimulatingAssetPriceUpdates(); });
  std::thread stockOrderThread([&]() { serv.startMessageProccesor(); });
  Cli cli;
  cli.loop();
  stockUpdaterThread.join();
  stockOrderThread.join();
}
