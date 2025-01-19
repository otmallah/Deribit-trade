# Connection Pool Documentation

## Purpose

The ConnectionPool manages a fixed pool of WebSocket connections to optimize resource usage and improve performance.

## Key Components

### Constructor

```cpp
ConnectionPool(size_t pool_size, const std::string &host, const std::string &port)
```

- Creates a pool of WebSocket connections
- Parameters:
  - pool_size: Maximum number of connections to maintain
  - host: Deribit server hostname
  - port: Server port number

### Connection Management

```cpp
WebSocketConnection *get_connection()
```

- Retrieves an available connection from the pool
- Returns nullptr if no connections are available
- Thread-safe operation

```cpp
void return_connection(WebSocketConnection *conn)
```

- Returns a connection to the pool for reuse
- Deletes connection if pool is at capacity
- Thread-safe operation

### Pool Initialization

```cpp
void initialize_pool(size_t size)
```

- Creates initial set of connections
- Handles connection creation failures
- Logs initialization status

## Usage Example

```cpp
// Create a pool of 10 connections
ConnectionPool pool(10, "test.deribit.com", "443");

// Get a connection
WebSocketConnection* conn = pool.get_connection();

// Use the connection
conn->SymbolSubscribe("BTC-PERPETUAL");

// Return connection to pool
pool.return_connection(conn);
```
