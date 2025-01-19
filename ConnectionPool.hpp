#pragma once
#include <queue>
#include <mutex>
#include "WebsocketConnection.hpp"

class ConnectionPool
{
public:
    ConnectionPool(size_t pool_size, const std::string &host, const std::string &port);
    ~ConnectionPool();

    WebSocketConnection *get_connection();
    void return_connection(WebSocketConnection *conn);

private:
    std::queue<WebSocketConnection *> available_connections;
    std::mutex pool_mutex;
    Logger logger;
    const std::string host;
    const std::string port;
    const size_t max_pool_size;

    void initialize_pool(size_t size);
};