
#include "Bank.h"
#include "Cli.h"
#include "stock/Server.h"

int main() {
  auto &serv = bank::server::Server::getInstance();
  std::thread stockUpdaterThread(
      [&]() { serv.startSimulatingStockPriceUpdates(); });
  std::thread stockOrderThread([&]() { serv.startMessageProccesor(); });
  Cli cli;
  cli.loop();
  stockUpdaterThread.join();
  stockOrderThread.join();
}
