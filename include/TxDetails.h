#ifndef BANK_TXDETAILS_H
#define BANK_TXDETAILS_H
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

#endif // BANK_TXDETAILS_H
