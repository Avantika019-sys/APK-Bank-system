#include <string>
#include <variant>
#ifndef BANK_TXDETAILS_H
#define BANK_TXDETAILS_H
struct assetPurchaseDetails {
  std::string assetName_;
  int assetsBought_;
  int pricePerAsset_;
};
struct assetSellDetails {
  std::string assetName_;
  int assetsSold_;
  int pricePerAsset_;
};
struct withdrawDetails {
  int amountWithdrawn_;
};
struct depositDetails {
  depositDetails(int amnt) : amountDepositted_(amnt) {}
  int amountDepositted_;
};
typedef std::variant<assetSellDetails, withdrawDetails, depositDetails,
                     assetPurchaseDetails>
    details;
;

#endif // BANK_TXDETAILS_H
