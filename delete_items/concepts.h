#pragma once
#include <concepts>
#include <type_traits>
#include <string>
#include <chrono>
#include <memory>

namespace banking::concepts {
    
// Concept for printable types
template<typename T>
concept Printable = requires(T t, std::ostream& os) {
    { os << t } -> std::same_as<std::ostream&>;
};

// Concept for numeric types
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// Concept for transaction-able objects
template<typename T>
concept Transactionable = requires(T t) {
    { t.get_id() } -> std::convertible_to<std::string>;
    { t.validate() } -> std::same_as<bool>;
    { t.process() } -> std::same_as<bool>;
};

// Concept for account-like types
template<typename T>
concept AccountLike = requires(T account, double amount) {
    typename T::AccountType;
    { account.deposit(amount) } -> std::same_as<bool>;
    { account.withdraw(amount) } -> std::same_as<bool>;
    { account.get_balance() } -> std::convertible_to<double>;
    { account.get_account_number() } -> std::convertible_to<std::string>;
};

// Value traits for different account types
template<typename T>
struct AccountTraits;

template<>
struct AccountTraits<class SavingsAccount> {
    using IdType = std::string;
    using BalanceType = double;
    static constexpr double interest_rate = 0.02;
    static constexpr const char* type_name = "Savings";
};

template<>
struct AccountTraits<class CheckingAccount> {
    using IdType = std::string;
    using BalanceType = double;
    static constexpr double overdraft_limit = 500.0;
    static constexpr const char* type_name = "Checking";
};

// Detection idiom for transaction fee
template<typename T, typename = void>
struct has_transaction_fee : std::false_type {};

template<typename T>
struct has_transaction_fee<T, std::void_t<decltype(T::transaction_fee)>> 
    : std::true_type {};

template<typename T>
constexpr bool has_transaction_fee_v = has_transaction_fee<T>::value;

} // namespace banking::concepts