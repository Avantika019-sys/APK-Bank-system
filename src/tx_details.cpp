#include "tx_details.h"
std::string ToString::operator()(const stockSellDetails &arg) {
  return "transaction type: stock sale\nPrice per stock: " +
         std::to_string(arg.pricePerStock_) +
         "\nAmount of stocks sold: " + std::to_string(arg.pricePerStock_) +
         "\nStock name: " + arg.stockName_;
}
std::string ToString::operator()(stockPurchaseDetails arg) {
  return "transaction type: stock purchase\nPrice per stock: " +
         std::to_string(arg.pricePerStock_) +
         "\nAmount of stocks bought: " + std::to_string(arg.stocksBought_) +
         "\nStock name: " + arg.stockName_;
}
std::string ToString::operator()(withdrawDetails arg) {
  return "transaction type: withdrawal\nAmount withdrawn: " +
         std::to_string(arg.amountWithdrawn_);
}
std::string ToString::operator()(depositDetails arg) {
  return "transaction type: deposit\nAmount depositted: " +
         std::to_string(arg.amountDepositted_);
}
