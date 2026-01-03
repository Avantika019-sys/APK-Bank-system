# Exchange system

| 202307397 | Ali Najafi |
| --------- | ---------- |
| 202207320 | Avantika   |
|           | Adriana    |

## SWAPK

# Introduction

This report describes our chosen project, the asset exchange system for SWAPK which is implemented using the C++ concepts learned throughout the course. The system functions as a multi-threaded trading platform, simulating asset prices, processing asynchronous buy and sell, managing user portfolios and maintaing detailed transaction history.

The primary goal of the project is to demonstrate how the C++ concepts taught in SWAPK can be used.

# Requirements

User perspective requirements:

- Withdrawing and depositting money in DKK currency, viewing transaction history and current balance
- Purchasing and selling assets of different types ( stock and crypto )
- Viewing portfolio stats, such as the portolfio value and trend
- Configuring stop loss limit rules, which allows the user to setup rules to automatically sell an asset if it goes below a certain price.

System perspective requirements:

- One server-node per asset type
- Server observability in the form of logs and metrics
- Simulating asset price changes
- The crypto server should handle events from crypto miners when they mine crypto

# Architecture

A user is the person using the application. A user can have one manager for each asset type. So one Crypto Manager and one Stock Manager per user. Managers are associated with an account, which is where they will get money to purchase, and put money after a sale.
A stock manager and crypto manager can use the same account.
The server is the source of truth for asset prices, managers does not store asset prices.

```plantuml
@startuml
actor User
' Define components
package "System" {
    [Manager] 
    [Account]
    [Server]
    [Miner]
    [Asset]
}

' Define relationships
Manager --> Server : Purchase/Sell
Manager"0..*" --> "1"Account : Creates transactions
Server --> Asset : Manages 1-N
User "1" -->"1" Account  : Deposit & withdraw
User "1"--> "0..*"Manager : Authorize Purchase/Sell
Miner --> Server : Mine event

@enduml
```

# Design

Managers and Miners can communicate with the server-nodes.
Each server-node has a message queue, where it can receive the following types of messages:

### Messages

#### InfoRequest

A manager can request information about a collection of assets, this is used for displaying portfolio stats and to display information about an asset to the user before the user authorizes the purchase or sale.

**Parameters**:

- Collection of asset symbols(e.g. BTC)

**Response**:

- Trend and current price for each asset

#### OrderRequest

If authorized by the user, then a manager places an order, which the server needs to confirm

**Parameters**:

- One asset symbol
- Type(either sale or purchase)
- Quantity in DKK

**Response**:

- Confirmation of order

#### MineEvent

Miners mine for crypto currencies, and when they succesfully mine a quantity of a crypto, then they emit events, which the server uses to adjust the total quantity of a crypto, for example there is around 19 million bitcoins, and if someone mines 100 bitcoins, then the server needs to know so it can update the total quantity and adjust the unit price.

**Parameters**:

**Response**:

#### Stop

The server can receive a stop message, which triggers graceful shutdown, allowing it to finish any ongoing requests.

### Request flow for placing an order

```plantuml
@startuml
User --> Manager : Purchase(assetSymbol, DKK)
Manager --> Server: InfoRequest(assetSymbol)
Server --> Manager: Price & Trend
Manager --> User: Display Price & Trend
User --> Manager: Authorize
Manager --> Server: OrderRequest(assetSymbol,DKK)
Server --> Manager: Confirmation
Manager --> Account: Add transaction & update balance
@enduml
```

The account class stores a collection of transactions, a transaction can have one of the following types:

- withdraw
- deposit
- asset purchase
- asset sale

# Implementation

## Asset classes

The asset classes consist of 2 classes:

- Stock
- Crypto

These classes store a vector of unit prices over time, the back of the vector is the latest price.
Additionally the Asset classes uses boost signals2, whenever a new price is added by the server, then the signal is triggered.

```cpp
typedef boost::signals2::signal<void(currency::DKK UpdatedPrice)> UpdateSignal;
```

## Message Queue

The Message Queue class is the primary interface to the server, its a template class which requires a Asset class as template type.
For the message queue we define the types of messages it can accept based on the template type

```cpp
template <typename T> using Message = exchange::trait::MessageQueue<T>::Variant;
```

With the use of traits we can define which messages are possible based on the asset type, this is a fixed trait

```cpp
template <typename T> class MessageQueue;

template <> struct MessageQueue<asset::Stock> {
  using Variant = std::variant<OrderRequest, InfoRequest, Stop>;
  static int QueueCapacity() { return 100; }
};

template <> struct MessageQueue<asset::Crypto> {
  using Variant = std::variant<OrderRequest, InfoRequest, Stop, MineEvent>;
  static int QueueCapacity() { return 80; }
};
```

We use std::variant to be able to define a set of types, this allows the queue to accept different types of messages, and based on the template asset type the set of messages can change. Overall this makes the queue very flexible.
Here we can see that the Crypto type has the additional MineEvent message type.

Additionally there is a value trait, the QueueCapacity() adds value information, based on the type of asset the server has different traffic, and requires the queue to have a certain capacity.

The queue is essentially the interface between a server thread and client threads. Therefore protection and synchronization mechanisms is important.
If a message producer tries to push a message to a full queue, then it should block the thread until there is space in the queue again.
If the server thread tries to pop a message, and the queue is empty, then it should be blocked until there is a message in the queue.
// NOTE husk at initalisere queue med queuecapacity
To achieve this we use a mutex and 2 condition variables

```cpp
  std::mutex mtx;
  std::condition_variable cv_not_empty;
  std::condition_variable cv_not_full;
```

## Server

The server is multi-threaded, it operates 2 threads:

#### Simulator thread

This thread regurlary iterates through assets, and generates a new price, within a percentage range of the current, and adds it to the assets unit price vector.

#### Message proccesor thread

## Account

The account

## Calculator/analyzer

When the server receives request to calculate trends it calls free functions defined in Calculator.h
These functions are template and takes a asset type,
Here we make use of alghoritm selection using tagging, depending on the amount of data that needs to proccesed we choose either a parallel or sequential calculation. The idea is that if the amount of data isnt very large then the overhead of spawning threads with std::async too large and doing it sequentially is faster.

## Logger

Each account has its own logger which logs to a file, which belongs to a account, The logger uses a **\*FILE**  pointer to write and read to files. The class uses RAII, when constructing the logger it aquires a file descriptor in the constructor and the destructor releases the file descriptor. This class has explicitly deleted the copy constructor and assignment operator, since we cant have 2 objects using the pointer, if one of the objects are destructed which closes the file descriptor and later the other object then tries write it will be an issue. Therefore we implemented the rule of 5, by implementing move semantics. now the FILE pointer can be moved from object to object.

## Meta programming

# Conclusion

