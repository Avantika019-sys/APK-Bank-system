
#include "asset/Server.h"
#include "asset/messages/Info.h"
#include <thread>

namespace asset::crypto {
class Miner {

public:
  Miner(std::string crypto, Server<types::Crypto> *serv)
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
      std::this_thread::sleep_for(std::chrono::seconds(5));
      serv->pushMsg(messages::crypto::MineEvent{crypto, distrib(gen)});
    }
  }
  Server<types::Crypto> *serv;
  std::string crypto;
  std::thread minerThread;
};
} // namespace asset::crypto
