#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include "WebsocketConnection.hpp"
#include <iostream>
#include <string>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = net::ip::tcp;

WebSocketConnection::WebSocketConnection(const std::string &host, const std::string &port)
    : host(host), port(port), ctx(ssl::context::tlsv12_client), ws(ioc, ctx), logger()
{
    connect();
    logger.log(LogLevel::INFO, "WebSocketConnection created");
}

WebSocketConnection::~WebSocketConnection()
{
    logger.log(LogLevel::INFO, "WebSocketConnection destroyed");
}

void WebSocketConnection::SymbolSubscribe(const std::string &symbol)
{
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

    logger.log(LogLevel::INFO, "Subscribing to channels");
    ws.write(net::buffer(subscribe_msg));
    logger.log(LogLevel::INFO, "Subscribed to channels");

    beast::flat_buffer buffer;
    while (true)
    {
        ws.read(buffer);
        std::cout << "Received: " << beast::make_printable(buffer.data()) << std::endl;
        buffer.consume(buffer.size());
    }
}

void WebSocketConnection::connect()
{
    tcp::resolver resolver{ioc};
    auto const results = resolver.resolve(host, port);

    // Connect to IP address
    auto ep = net::connect(get_lowest_layer(ws), results);
    logger.log(LogLevel::INFO, "Connected to: " + ep.address().to_string());

    // Perform SSL handshake
    ws.next_layer().handshake(ssl::stream_base::client);

    // Perform WebSocket handshake
    ws.handshake(host, "/ws/api/v2");
    logger.log(LogLevel::INFO, "Handshake completed");
}