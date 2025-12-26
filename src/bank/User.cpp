#include "bank/User.h"
#include "asset/types/Crypto.h"
#include "asset/types/Stock.h"
#include "bank/Account.h"
#include "util/Logger.h"
#include <boost/smart_ptr/make_shared_object.hpp>
namespace bank {
User::User(std::string name, std::string cpr,
           asset::Server<asset::types::Crypto> *cryptoServ,
           asset::Server<asset::types::Stock> *stockServ)
    : cpr_(cpr), logger(cpr_), account(boost::make_shared<Account>(&logger)),
      cryptoManager(cryptoServ ? new asset::Manager<asset::types::Crypto>(
                                     cryptoServ, account, &logger)
                               : nullptr),
      stockManager(stockServ ? new asset::Manager<asset::types::Stock>(
                                   stockServ, account, &logger)
                             : nullptr) {}
User::User(std::string name, std::string cpr)
    : User(name, cpr, nullptr, nullptr) {}
User::User(std::string name, std::string cpr,
           asset::Server<asset::types::Crypto> *cryptoServ)
    : User(name, cpr, cryptoServ, nullptr) {}

User::User(std::string name, std::string cpr,
           asset::Server<asset::types::Stock> *stockServ)
    : User(name, cpr, nullptr, stockServ) {}
std::string User::getCpr() { return cpr_; }
} // namespace bank
