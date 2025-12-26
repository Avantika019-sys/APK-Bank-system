#ifndef BANK_TXDETAILS_H
#define BANK_TXDETAILS_H
#include <string>
#include <variant>
struct stockPurchaseDetails {
  std::string stockName_;
  int stocksBought_;
  int pricePerStock_;
};
struct stockSellDetails {
  std::string stockName_;
  int stocksSold_;
  int pricePerStock_;
};
struct withdrawDetails {
  int amountWithdrawn_;
};
struct depositDetails {
  int amountDepositted_;
};
typedef std::variant<stockPurchaseDetails, stockSellDetails, withdrawDetails,
                     depositDetails>
    details;
;

#endif // BANK_TXDETAILS_H
