# Concurrent KV Store — Requirements

## 1. Project Overview

Concurrent KV Store is a single-node, in-memory key-value database written in modern C++20.

The project is designed to explore:

- Modern C++ programming
- Data structures
- Linux systems programming
- TCP networking
- Concurrency
- Synchronization
- Persistence
- Performance engineering
- Benchmarking

The system will expose a network interface through TCP and allow clients
to store, retrieve, delete, and inspect key-value pairs.

---

## 2. Goals

The primary goals are:

1. Build a functional in-memory key-value store.
2. Implement the storage engine in modern C++20.
3. Expose the store through a TCP server.
4. Support multiple concurrent clients.
5. Understand and implement synchronization correctly.
6. Investigate sharded storage for reducing lock contention.
7. Implement key expiration (TTL).
8. Implement persistence using WAL and snapshots.
9. Measure throughput and latency.
10. Investigate performance bottlenecks using profiling tools.
11. Produce a well-tested, documented, production-quality GitHub project.

---

## 3. Functional Requirements

### FR-1: Set a key

The server shall support:

    SET <key> <value>

Example:

    SET name Gaurav

The operation shall create a new key or replace the existing value.

---

### FR-2: Get a key

The server shall support:

    GET <key>

If the key exists, its value shall be returned.

If the key does not exist, the server shall return a not-found response.

---

### FR-3: Delete a key

The server shall support:

    DEL <key>

The operation shall remove the key if it exists.

The response shall indicate whether a key was deleted.

---

### FR-4: Check key existence

The server shall support:

    EXISTS <key>

The response shall indicate whether the key exists.

---

## 4. Future Functional Requirements

The following functionality will be introduced in later phases:

- EXPIRE
- TTL
- PERSIST
- MSET
- MGET
- INCR
- DECR
- SAVE
- Snapshot creation
- WAL-based recovery

These features are intentionally excluded from the initial MVP.

---

## 5. Data Model

The initial data model is:

    key   -> string
    value -> string

Conceptually:

    std::unordered_map<std::string, std::string>

The initial implementation will be single-threaded.

Concurrency will be introduced in later phases.

---

## 6. Limits

Initial limits:

- Maximum key size: 1 KiB
- Maximum value size: 1 MiB

The limits may become configurable in a later phase.

---

## 7. Networking Requirements

The server shall communicate with clients over TCP.

The initial protocol shall use a simple text-based command format.

Example:

    SET name Gaurav
    GET name
    DEL name
    EXISTS name

TCP shall be treated as a byte stream.

The implementation shall not assume that one send operation corresponds
to exactly one receive operation.

The protocol layer shall therefore handle:

- Partial commands
- Multiple commands received together
- Command buffering
- Malformed input
- Client disconnects

---

## 8. Concurrency Requirements

The final system shall support multiple concurrent clients.

The implementation shall:

- Avoid data races.
- Avoid undefined behavior caused by concurrent access.
- Protect shared state appropriately.
- Investigate lock contention.
- Support sharded storage.
- Measure the performance difference between synchronization strategies.

The project shall not introduce lock-free or other advanced techniques
unless there is a measurable reason to do so.

---

## 9. Persistence Requirements

The final system shall support persistence through:

- Write-Ahead Logging (WAL)
- Snapshots
- Recovery after restart

Persistence is not part of the initial MVP.

---

## 10. Performance Requirements

Performance shall be evaluated using:

### Throughput

Operations per second.

### Latency

At minimum:

- p50
- p95
- p99
- p99.9

Average latency alone shall not be considered sufficient.

Performance experiments shall compare relevant implementation choices,
including synchronization and sharding strategies.

---

## 11. Reliability Requirements

The server shall handle malformed or unexpected client input without
crashing.

The implementation shall be tested against:

- Empty commands
- Invalid commands
- Missing arguments
- Oversized keys
- Oversized values
- Partial TCP messages
- Multiple commands in one receive operation
- Client disconnects

---

## 12. Testing Requirements

The project shall contain:

- Unit tests
- Integration tests
- Concurrency tests
- Protocol tests
- Persistence/recovery tests
- Performance benchmarks

Testing will be introduced progressively as functionality is implemented.

---

## 13. Out of Scope

The project will not initially attempt to implement:

- Distributed clustering
- Replication
- Consensus protocols
- SQL
- Transactions
- Multi-region deployment
- Redis-compatible data structures
- Full Redis protocol compatibility
- Query language
- Authentication/authorization

These may be considered future extensions but are not part of the
core project.

---

## 14. Initial Success Criteria

The MVP is considered successful when a client can:

1. Connect to the server over TCP.
2. Execute SET.
3. Execute GET.
4. Execute DEL.
5. Execute EXISTS.
6. Receive correct responses.
7. Handle malformed requests without crashing.

The final project is considered successful when it additionally provides:

- Concurrent client handling
- Sharded storage
- TTL
- Persistence and recovery
- Automated tests
- Benchmarks
- Profiling results
- CI
- Complete documentation
