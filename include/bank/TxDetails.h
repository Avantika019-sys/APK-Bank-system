#include <string>
#include <variant>
#ifndef BANK_TXDETAILS_H
#define BANK_TXDETAILS_H
struct assetPurchaseDetails {
  std::string stockName_;
  int stocksBought_;
  int pricePerStock_;
};
struct assetSellDetails {
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
typedef std::variant<assetPurchaseDetails, assetSellDetails, withdrawDetails,
                     depositDetails>
    details;
;

#endif // BANK_TXDETAILS_H
