#include "asset/Manager.h"
#include "bank/Account.h"
#include "util/Logger.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <memory>
#ifndef BANK_USER_H
#define BANK_USER_H
using namespace std;
using namespace asset;
using namespace asset::types;
namespace bank {
class User {
public:
  User(string name, string cpr, boost::shared_ptr<Account> account,
       std::unique_ptr<Manager<Crypto>> cryptoManager,
       std::unique_ptr<Manager<Stock>> stockManager)
      : name_(name), cpr_(cpr), account(account), cryptoManager(cryptoManager),
        stockManager(stockManager) {}
  boost::shared_ptr<Account> account;
  std::unique_ptr<Manager<Crypto>> &cryptoManager;
  std::unique_ptr<Manager<Stock>> &stockManager;
  string getCpr() const { return cpr_; };

private:
  string cpr_;
  string name_;
};
} // namespace bank
#endif // BANK_USER_H
