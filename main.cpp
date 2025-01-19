#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <iostream>
#include <string>
#include "WebsocketConnection.hpp"
#include "DeribitTrader.hpp"
#include <nlohmann/json.hpp>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = net::ip::tcp;


int main()
{
    DeribitTrader trader("clientId", "clientSecret");
    // trader.SymbolSubscribe("BTC-PERPETUAL");
    nlohmann::json data = trader.get_order_book("BTC-PERPETUAL");
    std::cout << data.dump(4) << std::endl; // Pretty print with 4 spaces indent

    std::string order_id = trader.place_order("BTC-PERPETUAL", "buy", "1", "10000");
    std::cout << "Order ID: " << order_id << std::endl;

    trader.update_order(order_id, "2");
    trader.cancel_order(order_id);

    nlohmann::json positions = trader.get_positions();
    std::cout << positions.dump(4) << std::endl; // Pretty print with 4 spaces indent

    // get real time data market for subscrived symbol
    trader.SymbolSubscribe("BTC-PERPETUAL");

    return 0;
}