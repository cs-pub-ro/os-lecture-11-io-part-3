# IO (part 3): I/O Performance and Scalability

## 0. Pitch

- **demo**: different buffer size for writing data
- **demo**: read-write vs `splice` or `sendfile`

## 1. I/O Performance Metrics (and Issues)

- throughput: data processed per unit of time (Bps)
- latency: from initiation to start of processing (ms)
- bandwidth: rate of transmission (bps)
- response time: from request submission to first response (ms)
- scalability: maintain performance level as workload increases
- throughput vs bandwidth: throughput = actual, bandwidth = potential, theoretical
- latency vs response time: response time = latency + processing
- issues: bottleneck, queueing delay, congestion, contention
- **diagram**: process to disk: potential issues
- **diagram**: process to process (local): potential issues
- **diagram**: process to process (via network): potential issues

# 2. I/O Flow

- **diagram**: typical process-to-device flow
   - application buffer -> library calls -> copy from app buffer to middleware / libc buffer -> system call (transition to user mode) -> copy to kernel-mode buffer -> kernel flow - copy to driver-buffer -> hardware interrupt -> copy to actual device (controller)
- issues: multiple buffering, multiple copying, multiple layers
- why multiple buffering: batching, separation
- why multiple layers: portability, abstraction

## 3. I/O Multiplexing

- C10k problem: https://www.kegel.com/c10k.html
- **diagram**: serve multiple clients (simultaneously)
  - single thread monitors multiple connections
- using non-blocking I/O: do not block in operations (blocking I/O may get you blocked because some other thread "cleaned" the buffer)
- **diagram**: non-blocking I/O vs blocking I/O
- **demo**: using `epoll` with multiple clients

## 4. Asynchronous I/O

- non-blocking may return `EAGAIN` or `EWOULDBLOCK`
- async I/O means start the I/O operation and let it complete
- receive a notification and / or run a callback when async I/O operation is done
- common model in Node and Python server programs
- **demo**: Node, Python async operations
- **demo**: `aio_read` / `aio_write` / `libuv`
- mention `io_uring`

## 5. Zero-Copy

- reduce the double copying problem
- use information directly from the (kernel) buffer
- **diagram**: general overview of zero-copy
- `mmap`, `splice`, `sendfile`
- **demo**: `splice`
- **demo**: `sendfile`

## 6. Implementing Servers

- C10k problem: https://www.kegel.com/c10k.html
- multiprocess (prefork)
- multithread (thread pool)
- event-based (+ threads)
- **demo** with 3 approaches

## Conclusion and Takeaways

- I/O is a key topic in performance of data processing and serving to many clients / use cases.
- A typical I/O flow (from application to device, or from application to application via device) involves many layers and buffers that affect performance.
- I/O multiplexing calls (such as epoll) are used to scale I/O operations to many (simultaneous) I/O flows in the same thread.
- We combine non-blocking I/O with I/O multiplexing for best performance.
- Asynchronous I/O calls start and get completed, they use a callback to signal completion.
- We use zero-copy techniques to reduce double buffering time overhead.
- When implementing a server we have roughly 3 approaches (that can be combined): multiprocess, multithread, event-based.
