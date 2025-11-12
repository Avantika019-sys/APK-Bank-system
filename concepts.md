# Proposal for how we could divide stuff

## 1. Memory Management Package

### **Core C++ Concepts:**
- **PMR (Polymorphic Memory Resources)**
  - Custom `TrackingMemoryResource` inheriting from `std::pmr::memory_resource`
  - `std::pmr::unsynchronized_pool_resource` for memory pooling
  - PMR-aware containers throughout the system

### **Advanced Programming:**
- **Inheritance & Polymorphism**: Virtual function overrides in memory_resource
- **RAII**: Automatic resource management in MemoryPool
- **Static Members**: Singleton-like pattern for memory pool access

## 2. Concepts & Traits Package

### **C++20 Features:**
- **Concepts**: `Printable`, `Numeric`, `Transactionable`, `AccountLike`
- **Requires Expressions**: Constraint checking in concept definitions
- **Type Traits**: `AccountTraits` specializations for different account types

### **Template Metaprogramming:**
- **SFINAE**: Detection idiom with `void_t` and `has_transaction_fee`
- **Template Specialization**: Full specializations of `AccountTraits`
- **Compile-time Computation**: `static constexpr` members

## 3. Exceptions Package

### **Exception Handling:**
- **Custom Exception Hierarchy**: `BankingException` base with derived types
- **Exception Safety**: Strong exception guarantee implementations
- **RAII**: Automatic cleanup during stack unwinding

### **Advanced Features:**
- **Inheritance**: Exception type hierarchy
- **const Correctness**: Const member functions in exceptions
- **Move Semantics**: Efficient exception object handling

## 4. Core Banking Package

### **Object-Oriented Programming:**
- **Interface Design**: Pure virtual `Account` interface
- **CRTP (Curiously Recurring Template Pattern)**: `AccountWithStatistics`
- **Template Method Pattern**: `transfer()` method in base class

### **Advanced C++:**
- **Move Semantics**: Rule of Five in account classes
- **Perfect Forwarding**: In `AccountFactory` creation methods
- **constexpr if**: Type-specific operations in templates
- **RAII**: Automatic resource management in account operations

## 5. User Management Package

### **Modern C++ Features:**
- **Variadic Templates**: `add_accounts(AccountTypes&&...)`
- **Perfect Forwarding**: In `create_account` method
- **PMR Containers**: `pmr::vector` and `pmr::unordered_map`

### **Template Programming:**
- **Concepts Constraints**: `requires concepts::AccountLike<AccountType>`
- **Type Deduction**: `auto` return types with concepts
- **SFINAE**: In `get_accounts_of_type<T>()`

## 6. Card System Package

### **C++17/20 Features:**
- **std::variant**: `CardType` using variant of card types
- **Visitor Pattern**: `visit_cards` method with generic visitors
- **Chrono Library**: `year_month_day` for daily limit tracking

### **Generic Programming:**
- **Template Methods**: `find_card` with predicate templates
- **Lambda Expressions**: Used with visitor patterns
- **RAII**: Automatic resource management in card operations

## 7. Stock Trading Package

### **Advanced C++:**
- **Singleton Pattern**: `StockMarket` with deleted copy operations
- **Move Semantics**: `StockOrder` with efficient resource transfer
- **constexpr if**: In `get_portfolio_value<T>()` for type switching

### **STL Integration:**
- **Smart Pointers**: `unique_ptr` and ownership management
- **Containers**: `unordered_map` for stock storage, `vector` for orders
- **Algorithms**: Potential use with portfolio operations

### **Exception Safety:**
- **Strong Guarantee**: Stock operations with rollback capability
- **RAII**: Automatic cleanup of stock orders

## 8. Transaction System Package

### **C++17 Features:**
- **std::variant**: `TransactionData` holding multiple transaction types
- **Structured Bindings**: Potential use in transaction processing
- **Visitor Pattern**: Generic `process` method with visitors

### **Advanced Patterns:**
- **Strategy Pattern**: Different processing strategies per transaction type
- **Perfect Forwarding**: In `add_transaction` method
- **RAII**: Transaction lifecycle management

### **Generic Programming:**
- **Variadic Templates**: Flexible transaction creation
- **Template Specialization**: Implicit in variant usage
- **Type-safe Polymorphism**: Through std::variant and visitors

## Cross-Cutting Concepts Used Throughout:

### **C++11/14/17/20 Features:**
- **Auto Type Deduction**: Throughout codebase for cleaner syntax
- **Range-based for loops**: Container iteration
- **Lambda Expressions**: In algorithms and callbacks
- **Smart Pointers**: `unique_ptr`, `shared_ptr` for ownership
- **Move Semantics**: Rule of Five implementations
- **constexpr**: Compile-time computations and checks

### **STL Components:**
- **Containers**: `vector`, `unordered_map`, PMR variants
- **Algorithms**: `std::accumulate`, `std::remove_if`
- **Iterators**: Used with all container operations
- **Utilities**: `std::forward`, `std::move`, `std::exchange`

### **Memory Management:**
- **PMR Integration**: All major classes support polymorphic allocators
- **RAII**: Every resource-managing class follows RAII principles
- **Exception Safety**: Strong guarantees in critical operations

### **Template Metaprogramming:**
- **Type Traits**: Extensive use for compile-time type inspection
- **Concepts**: C++20 constraints on template parameters
- **SFINAE**: For conditional template instantiation
- **Variadic Templates**: For flexible interfaces

### **Design Patterns:**
- **Factory**: `AccountFactory` for object creation
- **Singleton**: `StockMarket` for global access
- **Visitor**: Transaction processing
- **Strategy**: Different processing algorithms
- **Template Method**: `Account::transfer()`
- **RAII**: Resource management throughout

