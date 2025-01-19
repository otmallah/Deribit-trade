# Deribit Trading Bot

A C++ trading bot for the Deribit cryptocurrency exchange that supports both REST API and WebSocket connections.

## Features

- Real-time market data streaming via WebSocket
- Order book retrieval
- Place/Cancel/Update orders
- Position management
- Authentication handling

## Dependencies

- Boost.Beast (WebSocket/SSL support)
- nlohmann-json (JSON parsing)
- CPR (HTTP requests)
- OpenSSL (SSL/TLS support)

### Installation (macOS)

```bash
brew install boost
brew install nlohmann-json
brew install cpr
brew install openssl
```

## Project Structure

- `main.cpp`: Main entry point for the trading bot.
- `DeribitTrader.hpp`: Header file for the DeribitTrader class.
- `DeribitTrader.cpp`: Implementation of the DeribitTrader class.
- `WebSocketConnection.hpp`: Header file for the WebSocketConnection class.
- `WebSocketConnection.cpp`: Implementation of the WebSocketConnection class.

## Build

```bash
g++ -std=c++17 main.cpp DeribitTrader.cpp WebSocketConnection.cpp \
-o out \
-lcpr -lssl -lcrypto -lboost_system \
-I/opt/homebrew/include \
-L/opt/homebrew/lib \
-Wl,-rpath,/opt/homebrew/lib
```


## Usage

```cpp
// Initialize trader with API credentials
DeribitTrader trader("your_client_id", "your_client_secret");
// Get order book
nlohmann::json orderbook = trader.get_order_book("BTC-PERPETUAL");
// Place order
std::string order_id = trader.place_order("BTC-PERPETUAL", "buy", "1", "10000");
// Cancel order
trader.cancel_order(order_id);
// Update order
trader.update_order(order_id, "2");
// Get positions
nlohmann::json positions = trader.get_positions();
// Subscribe to real-time market data
trader.SymbolSubscribe("BTC-PERPETUAL");
```

## WebSocket Connection

The WebSocket connection handles real-time market data streaming including:
- Order book updates
- Trade updates
- Ticker updates

## Note

This is a test implementation using the Deribit testnet. For production use, modify the base URL in DeribitTrader.cpp to use the main network.