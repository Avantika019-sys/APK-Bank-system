#include "asset/Manager.h"
#include "asset/Server.h"
#include "bank/Account.h"
#include "util/Logger.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#ifndef BANK_USER_H
#define BANK_USER_H
using namespace std;
using namespace asset;
using namespace asset::types;
namespace bank {
class User {
public:
  User(string name, string cpr, Server<Crypto> *cryptoServ,
       Server<Stock> *stockServ);
  User(string name, string cpr);
  User(string name, string cpr, Server<Crypto> *cryptoServ);
  User(string name, string cpr, Server<Stock> *stockServ);

  boost::shared_ptr<Account> account;
  Manager<Stock> *stockManager;
  Manager<Crypto> *cryptoManager;
  string getCpr() const;

private:
  string cpr_;
  util::Logger logger_;
  string name_;
};
} // namespace bank
#endif // BANK_USER_H
