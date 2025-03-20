`poll()` is a system call in the `<poll.h>` header used for monitoring multiple file descriptors to see if they have any pending I/O operations (read, write, or error conditions). It is an alternative to `select()`, but it scales better with a large number of file descriptors.  

### **Function Prototype:**
```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

### **Parameters:**
- `fds`: Pointer to an array of `pollfd` structures, each describing a file descriptor to monitor.
- `nfds`: Number of elements in the `fds` array.
- `timeout`: Time (in milliseconds) to wait:
  - `0` → Non-blocking mode (returns immediately).
  - `-1` → Wait indefinitely.
  - `> 0` → Waits for the specified milliseconds.

### **Return Values:**
- **`> 0`** → Number of file descriptors with events.
- **`0`** → Timeout expired.
- **`-1`** → Error occurred (`errno` is set).

### **Structure: `pollfd`**
```c
struct pollfd {
    int fd;         // File descriptor
    short events;   // Events to monitor
    short revents;  // Events returned
};
```

### **Events (`events` and `revents` Fields):**
| Macro        | Description |
|-------------|------------|
| `POLLIN`    | Data available to read. |
| `POLLOUT`   | Ready for writing. |
| `POLLERR`   | Error condition. |
| `POLLHUP`   | Hang-up detected. |
| `POLLNVAL`  | Invalid file descriptor. |

---

### **Example Usage:**
```c
#include <stdio.h>
#include <poll.h>
#include <unistd.h>

int main() {
    struct pollfd fds[1];
    fds[0].fd = 0;  // Monitor stdin (file descriptor 0)
    fds[0].events = POLLIN; // Check for input

    printf("Waiting for input...\n");
    int ret = poll(fds, 1, 5000); // Wait up to 5 seconds

    if (ret > 0) {
        if (fds[0].revents & POLLIN) {
            char buffer[100];
            read(0, buffer, sizeof(buffer)); // Read user input
            printf("You entered: %s", buffer);
        }
    } else if (ret == 0) {
        printf("Timeout! No input received.\n");
    } else {
        perror("poll");
    }

    return 0;
}
```

---

### **When to Use `poll()`?**
- When you need to monitor multiple file descriptors efficiently.
- When `select()` becomes inefficient due to large `FD_SETSIZE` limits.
- When working with socket servers or event-driven applications.

HTTP server! 🚀