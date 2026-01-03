#include "exchange/Miner.h"
namespace exchange {

Miner::Miner(std::string crypto, Server<asset::Crypto> &serv)
    : crypto(crypto), serv(serv),
      minerThread(std::thread([this]() { startMining(); })) {}
Miner::~Miner() {
  run = false;
  if (minerThread.joinable())
    minerThread.join();
}

void Miner::startMining() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> distrib(0.005, 0.012);
  while (run) {
    std::this_thread::sleep_for(5s);
    serv.pushMsg(message::MineEvent{crypto, distrib(gen)});
  }
}
} // namespace exchange
