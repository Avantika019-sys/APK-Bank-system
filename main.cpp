
#include "Cli.h"
#include "bank.h"
#include "stock/Server.h"

int main() {
  auto &serv = stock::Server::getInstance();
  std::thread stockUpdaterThread([&]() { serv.startUpdateStocksWorker(); });
  std::thread stockOrderThread([&]() { serv.startStockWorker(); });
  Cli cli;
  cli.loop();
  stockUpdaterThread.join();
  stockOrderThread.join();
}
