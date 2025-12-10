#pragma once
#include <string>
#include <memory>
#include <chrono>
#include <variant>
#include <vector>
#include "concepts.h"
#include "exceptions.h"

namespace banking {
    
// Card types as variant
struct DebitCard {};
struct CreditCard {};
struct PrepaidCard {};

using CardType = std::variant<DebitCard, CreditCard, PrepaidCard>;


// Concrete card implementations
class DebitCardImpl {
private:
    std::string card_number_;
    std::string linked_account_;
    double daily_limit_;
    double used_today_;
    std::chrono::year_month_day last_reset_;
    
public:
 virtual bool authorize_payment(double amount, const std::string& merchant) = 0;
    virtual std::string get_card_number() const = 0;
    virtual std::string get_card_type() const = 0;
    virtual double get_available_balance() const = 0;
    DebitCardImpl(std::string card_number, std::string linked_account, 
                  double daily_limit = 1000.0)
        : card_number_(std::move(card_number))
        , linked_account_(std::move(linked_account))
        , daily_limit_(daily_limit)
        , used_today_(0.0)
        , last_reset_(std::chrono::year_month_day{
            std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())}) {}
    
    bool authorize_payment(double amount, const std::string& merchant) override {
        reset_daily_usage_if_needed();
        
        if (amount > 0 && (used_today_ + amount) <= daily_limit_) {
            used_today_ += amount;
            return true;
        }
        return false;
    }
    
    std::string get_card_number() const override { return card_number_; }
    std::string get_card_type() const override { return "Debit"; }
    double get_available_balance() const override { return daily_limit_ - used_today_; }
    
private:
    void reset_daily_usage_if_needed() {
        auto today = std::chrono::year_month_day{
            std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())};
            
        if (today != last_reset_) {
            used_today_ = 0.0;
            last_reset_ = today;
        }
    }
};

// Card manager using visitor pattern
class CardManager {
private:
    std::vector<std::unique_ptr<Card>> cards_;
    
public:
    template<typename CardType, typename... Args>
    CardType& issue_card(Args&&... args) {
        auto card = std::make_unique<CardType>(std::forward<Args>(args)...);
        auto& ref = *card;
        cards_.push_back(std::move(card));
        return ref;
    }
    
    // Visitor for card operations
    template<typename Visitor>
    void visit_cards(Visitor&& visitor) {
        for (const auto& card : cards_) {
            std::forward<Visitor>(visitor)(*card);
        }
    }
    
    // Find card by predicate
    template<typename Predicate>
    Card* find_card(Predicate&& pred) {
        for (const auto& card : cards_) {
            if (std::forward<Predicate>(pred)(*card)) {
                return card.get();
            }
        }
        return nullptr;
    }
};

} // namespace banking