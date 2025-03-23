### Explanation of `run()` Function (Main Event Loop)

This function is the **core event loop** of your web server. It continuously monitors multiple sockets (both **server sockets** and **client sockets**) using the `poll()` system call. The goal is to handle new connections and process incoming client requests efficiently.

---

### **Breaking Down This Part:**
```cpp
// Process events
for (size_t i = 0; i < pollfds.size() && numEvents > 0; i++) {
    if (pollfds[i].revents & POLLIN) { 
        numEvents--; 
        if (isServerSocket(pollfds[i].fd)) { 
            // Server socket has activity - accept new connection
            int serverIdx = getServerIndex(pollfds[i].fd);
            acceptConnection(serverIdx);
        } else {
            // Client socket has activity - handle client
            handleClient(i);
        }
    }
}
```
---
### **1️⃣ Iterating Over `pollfds` (Monitored Sockets List)**
```cpp
for (size_t i = 0; i < pollfds.size() && numEvents > 0; i++) {
```
- `pollfds` is a list of **all monitored sockets**, including:
  - **Server sockets** (listening for new connections).
  - **Client sockets** (handling HTTP requests).
- The loop iterates through each monitored socket.
- The loop runs **only while `numEvents > 0`**, meaning only active events are processed.

---

### **2️⃣ Checking if There's Activity (`POLLIN` Event)**
```cpp
if (pollfds[i].revents & POLLIN) {
```
- `pollfds[i].revents` stores the **events that happened** on the socket.
- `POLLIN` means **"there is data to read"**:
  - For **server sockets**, this means a new client is trying to connect.
  - For **client sockets**, this means the client has sent data (HTTP request).

---

### **3️⃣ Handling Server Sockets (New Connections)**
```cpp
if (isServerSocket(pollfds[i].fd)) { 
    // Server socket has activity - accept new connection
    int serverIdx = getServerIndex(pollfds[i].fd);
    acceptConnection(serverIdx);
}
```
- If the **socket is a server socket**, it means a new client is trying to connect.
- `getServerIndex(pollfds[i].fd)`: Finds which server the connection is for.
- `acceptConnection(serverIdx)`: Accepts the new client and adds it to `pollfds` for monitoring.

---

### **4️⃣ Handling Client Sockets (Processing HTTP Requests)**
```cpp
else {
    // Client socket has activity - handle client
    handleClient(i);
}
```
- If the socket **is not a server socket**, it means an **existing client** sent a request.
- `handleClient(i)`: Reads the client's HTTP request and responds with the appropriate HTML page.

---

### **Summary**
✅ **Monitors multiple sockets** efficiently using `poll()` (no blocking).  
✅ **Accepts new client connections** if a server socket is ready.  
✅ **Reads and processes client HTTP requests** if a client socket is ready.  

This approach allows your server to handle **multiple connections at once** using a **single-threaded event-driven model**. 🚀### Explanation of `run()` Function (Main Event Loop)

This function is the **core event loop** of your web server. It continuously monitors multiple sockets (both **server sockets** and **client sockets**) using the `poll()` system call. The goal is to handle new connections and process incoming client requests efficiently.

---

### **Breaking Down This Part:**
```cpp
// Process events
for (size_t i = 0; i < pollfds.size() && numEvents > 0; i++) {
    if (pollfds[i].revents & POLLIN) { 
        numEvents--; 
        if (isServerSocket(pollfds[i].fd)) { 
            // Server socket has activity - accept new connection
            int serverIdx = getServerIndex(pollfds[i].fd);
            acceptConnection(serverIdx);
        } else {
            // Client socket has activity - handle client
            handleClient(i);
        }
    }
}
```
---
### **1️⃣ Iterating Over `pollfds` (Monitored Sockets List)**
```cpp
for (size_t i = 0; i < pollfds.size() && numEvents > 0; i++) {
```
- `pollfds` is a list of **all monitored sockets**, including:
  - **Server sockets** (listening for new connections).
  - **Client sockets** (handling HTTP requests).
- The loop iterates through each monitored socket.
- The loop runs **only while `numEvents > 0`**, meaning only active events are processed.

---

### **2️⃣ Checking if There's Activity (`POLLIN` Event)**
```cpp
if (pollfds[i].revents & POLLIN) {
```
- `pollfds[i].revents` stores the **events that happened** on the socket.
- `POLLIN` means **"there is data to read"**:
  - For **server sockets**, this means a new client is trying to connect.
  - For **client sockets**, this means the client has sent data (HTTP request).

---

### **3️⃣ Handling Server Sockets (New Connections)**
```cpp
if (isServerSocket(pollfds[i].fd)) { 
    // Server socket has activity - accept new connection
    int serverIdx = getServerIndex(pollfds[i].fd);
    acceptConnection(serverIdx);
}
```
- If the **socket is a server socket**, it means a new client is trying to connect.
- `getServerIndex(pollfds[i].fd)`: Finds which server the connection is for.
- `acceptConnection(serverIdx)`: Accepts the new client and adds it to `pollfds` for monitoring.

---

### **4️⃣ Handling Client Sockets (Processing HTTP Requests)**
```cpp
else {
    // Client socket has activity - handle client
    handleClient(i);
}
```
- If the socket **is not a server socket**, it means an **existing client** sent a request.
- `handleClient(i)`: Reads the client's HTTP request and responds with the appropriate HTML page.

---

### **Summary**
✅ **Monitors multiple sockets** efficiently using `poll()` (no blocking).  
✅ **Accepts new client connections** if a server socket is ready.  
✅ **Reads and processes client HTTP requests** if a client socket is ready.  

This approach allows your server to handle **multiple connections at once** using a **single-threaded event-driven model**. 🚀
