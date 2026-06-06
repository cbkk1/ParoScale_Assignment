# Design Rationale

**Language.** C++17, standard library + POSIX sockets only. No external
libraries; the socket API is the standard way to do networking in C++.

**Data structure.** Two hash maps: package to its dependencies, and package to a
count of how many packages depend on it. The count answers `REMOVE`'s question —
"does anything depend on this?" — in O(1). A reverse map (the *set* of
dependents) was considered, but only the count is needed, so it is the minimal
sufficient structure.

**Concurrency.** One `std::shared_mutex` over the whole index (`QUERY` reads,
`INDEX`/`REMOVE` write). Every operation spans several packages, so the
consistency unit is the whole graph, not one entry; per-package locks would need
multi-lock ordering to avoid deadlock. Each critical section is a few hash
lookups, so the single lock is not a bottleneck at the tested concurrency. The
scaling path, if ever needed, is sharding — not per-package locks.

**Connections.** One thread per connection. Bytes from `recv` are buffered and
split on `\n`. `send` uses `MSG_NOSIGNAL`, so a client disconnecting mid-write
returns an error instead of killing the process via `SIGPIPE`. An event loop
(`epoll`/`io_uring`) would scale further but adds complexity unneeded here.

**Correctness.** Each response depends only on current index state, not arrival
order, so any concurrent interleaving stays consistent and the harness converges.

**Omitted.** Graceful shutdown on `SIGTERM` — appropriate for a real deployment,
unnecessary for the test.
