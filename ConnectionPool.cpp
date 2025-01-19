#include "ConnectionPool.hpp"

ConnectionPool::ConnectionPool(size_t pool_size, const std::string &host, const std::string &port)
    : host(host), port(port), max_pool_size(pool_size), logger()
{
    initialize_pool(pool_size);
}

void ConnectionPool::initialize_pool(size_t size)
{
    std::lock_guard<std::mutex> lock(pool_mutex);
    for (size_t i = 0; i < size; ++i)
    {
        try
        {
            auto conn = new WebSocketConnection(host, port);
            available_connections.push(conn);
        }
        catch (const std::exception &e)
        {
            logger.log(LogLevel::ERROR, "Failed to initialize connection: " + std::string(e.what()));
        }
    }
    logger.log(LogLevel::INFO, "Connection pool initialized with " + std::to_string(size) + " connections");
}

WebSocketConnection *ConnectionPool::get_connection()
{
    std::lock_guard<std::mutex> lock(pool_mutex);
    if (available_connections.empty())
    {
        logger.log(LogLevel::WARN, "No available connections in pool");
        return nullptr;
    }

    auto conn = available_connections.front();
    available_connections.pop();
    return conn;
}

void ConnectionPool::return_connection(WebSocketConnection *conn)
{
    std::lock_guard<std::mutex> lock(pool_mutex);
    if (available_connections.size() < max_pool_size)
    {
        available_connections.push(conn);
    }
    else
    {
        delete conn;
    }
}

ConnectionPool::~ConnectionPool()
{
    std::lock_guard<std::mutex> lock(pool_mutex);
    while (!available_connections.empty())
    {
        auto conn = available_connections.front();
        available_connections.pop();
        delete conn;
    }
}