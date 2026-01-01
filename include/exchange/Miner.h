
#include "Server.h"
#include <thread>

#ifndef EXCHANGE_CRYPTOMINER_H
#define EXCHANGE_CRYPTOMINER_H
using namespace std::chrono_literals;
namespace exchange {
class Miner {

public:
  Miner(std::string crypto, Server<asset::Crypto> &serv)
      : crypto(crypto), serv(serv),
        minerThread(std::thread([this]() { startMining(); })) {}
  ~Miner() {
    run = false;
    if (minerThread.joinable())
      minerThread.join();
  }

private:
  std::atomic<bool> run{true};
  void startMining() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distrib(0.005, 0.012);
    while (run) {
      std::this_thread::sleep_for(5s);
      serv.pushMsg(message::MineEvent{crypto, distrib(gen)});
    }
  }
  Server<asset::Crypto> &serv;
  std::string crypto;
  std::thread minerThread;
};
} // namespace exchange
#endif // EXCHANGE_CRYPTOMINER_H
