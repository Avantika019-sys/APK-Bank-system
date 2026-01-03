
#include "Server.h"
#include <thread>

#ifndef EXCHANGE_CRYPTOMINER_H
#define EXCHANGE_CRYPTOMINER_H
using namespace std::chrono_literals;
namespace exchange {
class Miner {

public:
  Miner(std::string crypto, Server<asset::Crypto> &serv);
  ~Miner();

private:
  std::atomic<bool> run{true};
  void startMining();
  Server<asset::Crypto> &serv;
  std::string crypto;
  std::thread minerThread;
};
} // namespace exchange
#endif // EXCHANGE_CRYPTOMINER_H
