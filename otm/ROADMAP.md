A roadmap to create a **non-blocking multiplexer server** in C++98 using the allowed functions. Focus on **concepts** and **system calls** to study, not code:

---

### **1. Master the Basics of Socket Programming**
- **Study**:
  - TCP/IP fundamentals (connection setup, client-server model).
  - Socket API: `socket()`, `bind()`, `listen()`, `accept()`, `connect()`.
  - Address resolution: `getaddrinfo()`, `freeaddrinfo()`.
  - Error handling: `errno`, `strerror()`, `gai_strerror()`.
- **Goal**: Create a basic blocking server that handles one client at a time.

---

### **2. Learn Non-Blocking I/O**
- **Study**:
  - `fcntl()` to set `O_NONBLOCK` on sockets.
  - Implications of non-blocking mode for `accept()`, `read()`, `write()`.
  - Handling `EAGAIN`/`EWOULDBLOCK` errors.
- **Goal**: Modify the server to use non-blocking sockets.

---

### **3. Choose a Multiplexing API**
Pick **one** of the following (recommended: `epoll` or `kqueue` for scalability):
- **`select()`**:
  - Study `fd_set`, `FD_ZERO`, `FD_SET`, `select()`.
  - Limitations: FD_SETSIZE limit, linear scan.
- **`poll()`**:
  - Study `struct pollfd`, `poll()`.
  - Better than `select()` but still O(n) complexity.
- **`epoll` (Linux)**:
  - Study `epoll_create()`, `epoll_ctl()`, `epoll_wait()`.
  - Edge-triggered (ET) vs. level-triggered (LT) modes.
- **`kqueue` (BSD/macOS)**:
  - Study `kqueue()`, `kevent()`, event filters (`EVFILT_READ`, `EVFILT_WRITE`).

---

### **4. Implement the Multiplexer**
- **Steps**:
  1. Create a listening socket and set it to non-blocking.
  2. Initialize the multiplexer (e.g., `epoll_create()`).
  3. Register the listening socket with the multiplexer to monitor `EPOLLIN` (or equivalent).
  4. Enter an event loop:
     - Wait for events (e.g., `epoll_wait()`).
     - For each event:
       - **New connection**: `accept()` the client, set non-blocking, add to multiplexer.
       - **Data available**: `read()` from client, process, and `write()` response.
       - **Error/closure**: Remove the client from the multiplexer and `close()`.

---

### **5. Handle Client Communication**
- **Study**:
  - Partial reads/writes (buffer management).
  - How to detect closed connections (`read()` returns 0).
  - Avoiding blocking in `send()`/`recv()` using non-blocking sockets.
- **Goal**: Ensure the server can handle multiple clients simultaneously without blocking.

---

### **6. Edge Cases & Cleanup**
- **Study**:
  - Signal handling (e.g., `SIGPIPE` when writing to closed sockets).
  - Graceful shutdown: `close()` sockets, free resources.
  - Resource leaks (always `close()` file descriptors).
- **Goal**: Make the server robust against errors and crashes.

---

### **7. Testing & Optimization**
- **Test**:
  - Simultaneous connections (e.g., using `ab` or `wrk`).
  - Stress-test with slow clients or partial data.
- **Optimize**:
  - Use edge-triggered mode (for `epoll`/`kqueue`) to reduce syscalls.
  - Reuse buffers and minimize system call overhead.

---

### **Key Resources to Study**
1. **Man Pages**:
   - `man 2 epoll_ctl`, `man 2 epoll_wait`, `man 2 select`, `man 2 poll`.
   - `man 7 epoll` (overview of `epoll`).
2. **Books**:
   - *Unix Network Programming* by Richard Stevens (Volume 1).
   - *The Linux Programming Interface* by Michael Kerrisk (Chapters on I/O multiplexing).
3. **Online**:
   - [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/).
   - Linux `epoll` tutorial (e.g., [https://linux.die.net/man/7/epoll](https://linux.die.net/man/7/epoll)).

---

### **Final Checklist**
- [ ] Server uses non-blocking sockets.
- [ ] Multiplexer (e.g., `epoll`) monitors all sockets.
- [ ] Event loop handles new connections and data events.
- [ ] Graceful error handling and cleanup.
- [ ] Tested with concurrent clients.

This roadmap ensures you build a scalable, non-blocking server using I/O multiplexing. Focus on understanding the system calls and their interactions!