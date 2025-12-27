#include "asset/Manager.h"
#include "asset/Server.h"
#include "bank/Account.h"
#include "util/Logger.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#ifndef BANK_USER_H
#define BANK_USER_H
namespace bank {
class User {
public:
  User(std::string name, std::string cpr,
       asset::Server<asset::types::Crypto> *cryptoServ,
       asset::Server<asset::types::Stock> *stockServ);
  User(std::string name, std::string cpr);
  User(std::string name, std::string cpr,
       asset::Server<asset::types::Crypto> *cryptoServ);
  User(std::string name, std::string cpr,
       asset::Server<asset::types::Stock> *stockServ);
  boost::shared_ptr<Account> account;
  asset::Manager<asset::types::Stock> *stockManager;
  asset::Manager<asset::types::Crypto> *cryptoManager;
  std::string getCpr() const;

private:
  std::string cpr_;
  util::Logger logger;
  asset::Server<asset::types::Crypto> *cryptoServ;
  asset::Server<asset::types::Stock> *stockServ;
  std::string name_;
};
} // namespace bank
#endif // BANK_USER_H
