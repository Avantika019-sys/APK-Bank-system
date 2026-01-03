
#include "Server.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <thread>

#ifndef EXCHANGE_CRYPTOMINER_H
#define EXCHANGE_CRYPTOMINER_H
using namespace std::chrono_literals;
namespace exchange {
class Miner {

public:
  Miner(std::string crypto, boost::shared_ptr<Server<asset::Crypto>> serv);
  ~Miner();

private:
  std::atomic<bool> run{true};
  void startMining();
  boost::shared_ptr<Server<asset::Crypto>> serv;
  std::string crypto;
  std::thread minerThread;
};
} // namespace exchange
#endif // EXCHANGE_CRYPTOMINER_H
