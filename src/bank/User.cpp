#include "bank/User.h"
#include "asset/types/Crypto.h"
#include "asset/types/Stock.h"
#include "bank/Account.h"
#include "util/Logger.h"
#include <boost/smart_ptr/make_shared_object.hpp>
using namespace std;
using namespace asset;
using namespace asset::types;
namespace bank {
User::User(string name, string cpr, Server<Crypto> *cryptoServ,
           Server<Stock> *stockServ)
    : cpr_(cpr), logger_(cpr_), account(boost::make_shared<Account>(&logger_)),
      cryptoManager(cryptoServ
                        ? new Manager<Crypto>(cryptoServ, account, &logger_)
                        : nullptr),
      stockManager(stockServ ? new Manager<Stock>(stockServ, account, &logger_)
                             : nullptr) {}
User::User(string name, string cpr) : User(name, cpr, nullptr, nullptr) {}
User::User(string name, string cpr, Server<Crypto> *cryptoServ)
    : User(name, cpr, cryptoServ, nullptr) {}

User::User(string name, string cpr, Server<Stock> *stockServ)
    : User(name, cpr, nullptr, stockServ) {}
string User::getCpr() const { return cpr_; }
} // namespace bank
