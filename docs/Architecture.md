# Deribit Trading Bot Architecture

## Core Components

### DeribitTrader

The main trading interface that handles:

- Authentication with Deribit API
- Order management (place/cancel/modify)
- Market data retrieval
- Position management

### WebSocketConnection

Handles real-time market data streaming:

- Secure WebSocket connection management
- SSL/TLS handshake
- Market data subscription
- Message handling

### ConnectionPool

Manages a pool of WebSocket connections:

- Efficient connection reuse
- Thread-safe connection management
- Automatic cleanup of unused connections

### Logger

Provides structured logging across the application:

- Different log levels (INFO, WARN, ERROR)
- Timestamp and context information
- Thread-safe logging operations

## Data Flow

1. DeribitTrader initializes with API credentials
2. ConnectionPool creates and manages WebSocket connections
3. WebSocketConnection handles real-time market data
4. Logger provides visibility into system operations
