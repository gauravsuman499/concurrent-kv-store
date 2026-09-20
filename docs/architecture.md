# Concurrent KV Store — Architecture

## 1. Architecture Overview

The system is organized into separate layers:

    Client
      |
      | TCP
      v
    Network Layer
      |
      v
    Protocol Layer
      |
      v
    Command Engine
      |
      v
    Storage Engine
      |
      +---- TTL Manager
      |
      +---- Persistence Layer
             |
             +---- WAL
             |
             +---- Snapshots

Each layer has a defined responsibility and should minimize knowledge
of the implementation details of other layers.

---

## 2. Network Layer

The network layer is responsible for communication with clients.

Responsibilities:

- Create TCP socket
- Bind to address
- Listen for connections
- Accept clients
- Receive bytes
- Send responses
- Detect client disconnects
- Manage client connections

The network layer shall not directly manipulate the key-value store.

---

## 3. Protocol Layer

The protocol layer converts raw TCP bytes into commands and converts
command results into responses.

Responsibilities:

- Buffer incoming bytes
- Detect complete commands
- Parse commands
- Validate command structure
- Handle malformed requests
- Format responses

The protocol layer must account for TCP being a byte stream.

A single receive operation may contain:

- A partial command
- One complete command
- Multiple commands

---

## 4. Command Engine

The command engine maps parsed commands to storage operations.

Example:

    SET name Gaurav

becomes conceptually:

    store.set("name", "Gaurav");

The command engine is responsible for:

- Command dispatch
- Argument validation
- Calling appropriate storage operations
- Creating command-level responses

---

## 5. Storage Engine

The storage engine owns the key-value data.

Initial implementation:

    std::unordered_map<std::string, std::string>

Responsibilities:

- SET
- GET
- DEL
- EXISTS

The storage engine shall not depend on the TCP/networking layer.

This separation allows the storage engine to be tested independently.

---

## 6. Concurrency Model

The first storage implementation will be single-threaded.

Concurrency will be introduced progressively.

### Version 1

    One thread
       |
       v
    unordered_map

### Version 2

    Multiple threads
          |
          v
    Global mutex
          |
          v
    unordered_map

### Version 3

    Multiple threads
          |
          v
      Hash(key)
          |
       +--+--+--+
       |  |  |  |
       v  v  v  v
     Shard 0 ... Shard N

Each shard owns:

- Its own key-value container
- Its own synchronization primitive

This reduces contention between operations involving unrelated keys.

---

## 7. Sharding

A key will be mapped to a shard using a hash function.

Conceptually:

    hash = hash(key)
    shard_id = hash % shard_count

The same key must always map to the same shard during the lifetime
of the store configuration.

Example:

    "name"
       |
       v
    hash("name")
       |
       v
    shard_id = hash % 16
       |
       v
    Shard 5

The exact number of shards will be configurable or selected during
implementation.

---

## 8. Event Handling

The final networking architecture will use Linux non-blocking sockets
and epoll for event-driven I/O.

Conceptually:

                    Clients
                       |
                       v
                +-------------+
                | TCP Server  |
                +------+------+
                       |
                       v
                +-------------+
                |   epoll     |
                +------+------+
                       |
                       v
                +-------------+
                | Worker Pool |
                +------+------+
                       |
                       v
                +-------------+
                | Command     |
                | Engine      |
                +------+------+
                       |
                       v
                +-------------+
                | Sharded     |
                | KV Store    |
                +-------------+

This architecture will be introduced only after the simpler
single-threaded implementation is working.

---

## 9. TTL

TTL functionality will allow a key to expire automatically.

Conceptually:

    key
     |
     +-- value
     |
     +-- expiration timestamp

An expired key shall behave as if it does not exist.

TTL implementation details will be decided in the TTL phase.

---

## 10. Persistence

Persistence will eventually consist of two mechanisms.

### Write-Ahead Log

Mutating operations will be recorded so that the store can reconstruct
its state after a restart.

Conceptually:

    SET name Gaurav
        |
        v
      WAL
        |
        v
    Memory Store

### Snapshot

A snapshot will periodically serialize the current state.

Conceptually:

    Memory Store
        |
        v
     Snapshot
        |
        v
    Disk File

The recovery process will use the snapshot and WAL as appropriate.

---

## 11. Error Handling

Errors shall be handled at the appropriate layer.

Examples:

    Network error
        -> Network layer

    Malformed command
        -> Protocol layer

    Invalid command arguments
        -> Command engine

    Missing key
        -> Storage result

The system should return controlled errors rather than terminate
the server because of malformed client input.

---

## 12. Testing Architecture

The architecture allows independent testing of each layer.

### Unit tests

Storage and protocol components can be tested without networking.

### Integration tests

A real server can be started and tested through TCP.

### Concurrency tests

Multiple threads can exercise the storage engine concurrently.

### Persistence tests

The server can be stopped and restarted to verify recovery.

### Benchmarks

Storage and network performance can be measured independently.

---

## 13. Architectural Principles

The project follows these principles:

1. Prefer simple designs before complex designs.
2. Separate responsibilities between components.
3. Introduce concurrency only when required.
4. Measure performance before optimizing.
5. Avoid premature lock-free programming.
6. Avoid unnecessary custom memory allocators.
7. Prefer standard C++ facilities where appropriate.
8. Document important design trade-offs.
9. Test behavior before optimizing implementation details.
10. Keep the system incrementally buildable.

---

## 14. Architecture Evolution

The system will evolve through the following stages:

    Single-threaded Storage
            |
            v
    Command Engine
            |
            v
    TCP Server
            |
            v
    Concurrent Server
            |
            v
    Sharded Storage
            |
            v
    TTL
            |
            v
    Persistence
            |
            v
    Benchmarking
            |
            v
    Production-quality Repository
