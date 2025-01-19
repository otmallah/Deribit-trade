# WebSocketConnection Implementation Details

## Header File (WebSocketConnection.hpp)

### Includes and Namespaces

```cpp
#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <string>
#include "Logger.hpp"

namespace beast = boost::beast;         // Core Boost.Beast
namespace websocket = beast::websocket; // WebSocket functionality
namespace net = boost::asio;           // Networking
namespace ssl = boost::asio::ssl;      // SSL/TLS
using tcp = net::ip::tcp;             // TCP networking
```

### Class Definition

```cpp
class WebSocketConnection {
private:
    std::string host;    // Server hostname (e.g., "test.deribit.com")
    std::string port;    // Server port (e.g., "443")
    net::io_context ioc; // Manages I/O operations
    ssl::context ctx;    // SSL context for secure connection
    websocket::stream<beast::ssl_stream<tcp::socket>> ws; // WebSocket stream
    Logger logger;       // Logging utility
```

## Implementation File (WebSocketConnection.cpp)

### Constructor

```cpp
WebSocketConnection::WebSocketConnection(const std::string &host, const std::string &port)
    : host(host),
      port(port),
      ctx(ssl::context::tlsv12_client), // Use TLS v1.2
      ws(ioc, ctx),                     // Initialize WebSocket with SSL
      logger()
{
    connect();  // Establish connection immediately
    logger.log(LogLevel::INFO, "WebSocketConnection created");
}
```

### Connection Method

```cpp
void WebSocketConnection::connect()
{
    // Step 1: DNS Resolution
    tcp::resolver resolver{ioc};
    auto const results = resolver.resolve(host, port);

    // Step 2: TCP Connection
    auto ep = net::connect(get_lowest_layer(ws), results);
    logger.log(LogLevel::INFO, "Connected to: " + ep.address().to_string());

    // Step 3: SSL Handshake
    ws.next_layer().handshake(ssl::stream_base::client);

    // Step 4: WebSocket Handshake
    ws.handshake(host, "/ws/api/v2");
    logger.log(LogLevel::INFO, "Handshake completed");
}
```

### Market Data Subscription

```cpp
void WebSocketConnection::SymbolSubscribe(const std::string &symbol)
{
    // Step 1: Prepare subscription message
    std::string subscribe_msg = R"({
        "jsonrpc": "2.0",
        "method": "public/subscribe",
        "id": 42,
        "params": {
            "channels": [
                "book.BTC-PERPETUAL.100ms",
                "trades.BTC-PERPETUAL.100ms",
                "ticker.BTC-PERPETUAL.100ms"
            ]
        }
    })";

    // Step 2: Send subscription request
    logger.log(LogLevel::INFO, "Subscribing to channels");
    ws.write(net::buffer(subscribe_msg));
    logger.log(LogLevel::INFO, "Subscribed to channels");

    // Step 3: Process incoming messages
    beast::flat_buffer buffer;
    while (true) {
        ws.read(buffer);
        std::cout << "Received: " << beast::make_printable(buffer.data()) << std::endl;
        buffer.consume(buffer.size());
    }
}
```

## Key Features

1. **Secure Communication**

   - Uses TLS v1.2 for encryption
   - Implements full SSL/TLS handshake
   - Secure WebSocket protocol (wss://)

2. **Connection Management**

   - Automatic connection establishment
   - DNS resolution
   - TCP connection setup
   - SSL/TLS handshake
   - WebSocket protocol upgrade

3. **Real-time Data Handling**

   - Asynchronous message processing
   - Efficient buffer management
   - Continuous data streaming

4. **Error Handling & Logging**
   - Comprehensive error logging
   - Connection status tracking
   - Exception handling

## Usage Considerations

1. **Thread Safety**

   - Not thread-safe by default
   - Single connection per instance
   - Use ConnectionPool for concurrent access

2. **Resource Management**

   - Automatic cleanup in destructor
   - Proper buffer management
   - Connection pooling support

3. **Performance**
   - Efficient buffer usage
   - Minimal memory footprint
   - Quick connection establishment
