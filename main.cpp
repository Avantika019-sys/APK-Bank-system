
#include "Bank.h"
#include "Cli.h"
#include "stock/Server.h"

int main() {
  auto &serv = bank::stock::Server::getInstance();
  std::thread stockUpdaterThread([&]() { serv.startUpdateStocksWorker(); });
  std::thread stockOrderThread([&]() { serv.startStockWorker(); });
  Cli cli;
  cli.loop();
  stockUpdaterThread.join();
  stockOrderThread.join();
}
