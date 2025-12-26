
#include "./include/asset/Crypto.h"
#include "Cli.h"
#include "Logger.h"
#include "server/Server.h"

int main() {
  // Logger l("1234");
  // l.log("this is a log message", level::INFO, field("tx", 123),
  //       field("field2", 1.0));
  // l.log("message 2", level::INFO, field("tx", 123), field("field2", 1.0));
  auto &serv = bank::server::Server<Crypto>::getInstance();
  std::thread stockUpdaterThread(
      [&]() { serv.startSimulatingAssetPriceUpdates(); });
  std::thread stockOrderThread([&]() { serv.startMessageProccesor(); });
  Cli cli;
  cli.loop();
  stockUpdaterThread.join();
  stockOrderThread.join();
}
