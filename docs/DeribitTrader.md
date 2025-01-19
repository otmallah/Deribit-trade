# Deribit Trader Documentation

## Purpose

Main interface for interacting with Deribit's trading API, handling both REST and WebSocket communications.

## Key Features

### Authentication

```cpp
std::string get_access_token()
```

- Handles API authentication
- Manages access tokens
- Automatic token refresh

### Order Management

```cpp
std::string place_order(const std::string &instrument_name,
                       const std::string &side,
                       const std::string &quantity,
                       const std::string &price)
```

- Places new orders
- Returns order ID
- Handles errors

```cpp
void cancel_order(const std::string &order_id)
```

- Cancels existing orders
- Validates order status

```cpp
void update_order(const std::string &order_id,
                 const std::string &quantity)
```

- Modifies existing orders
- Updates order parameters

### Market Data

```cpp
nlohmann::json get_order_book(const std::string &instrument_name)
```

- Retrieves current order book
- Returns structured JSON data

## Usage Example

```cpp
DeribitTrader trader("client_id", "client_secret");
auto orderbook = trader.get_order_book("BTC-PERPETUAL");
std::string order_id = trader.place_order("BTC-PERPETUAL", "buy", "1", "50000");
```
