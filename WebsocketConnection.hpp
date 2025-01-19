#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <string>
#include "Logger.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = net::ip::tcp;

class WebSocketConnection
{
public:
    WebSocketConnection(const std::string &host, const std::string &port);
    ~WebSocketConnection();

    void connect();
    void SymbolSubscribe(const std::string &symbol);

private:
    std::string host;
    std::string port;
    net::io_context ioc;
    ssl::context ctx{ssl::context::tlsv12_client};
    websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};
    Logger logger;
};
