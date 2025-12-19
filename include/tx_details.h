#ifndef BANK_TX_DETAILS_H
#define BANK_TX_DETAILS_H
#include <string>
#include <variant>
struct stockPurchaseDetails {
  std::string stockName_;
  uint stocksBought_;
  uint pricePerStock_;
};
struct stockSellDetails {
  std::string stockName_;
  uint stocksSold_;
  uint pricePerStock_;
};
struct withdrawDetails {
  uint amountWithdrawn_;
};
struct depositDetails {
  uint amountDepositted_;
};
// enum stock, buy sell
typedef std::variant<stockPurchaseDetails, stockSellDetails, withdrawDetails,
                     depositDetails>
    details;
;
struct ToString {
  std::string operator()(const stockSellDetails &arg);
  std::string operator()(stockPurchaseDetails arg);
  std::string operator()(withdrawDetails arg);
  std::string operator()(depositDetails arg);
};

#endif // BANK_TX_DETAILS_H
