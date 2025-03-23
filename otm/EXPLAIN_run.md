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

### **What Does "Active Events Are Processed" Mean?**  
In the `run()` function, your server is using the `poll()` system call to **monitor multiple sockets** (both **server sockets** and **client sockets**).  

🔹 **Active events** refer to **sockets that are ready for some action**—meaning there is something to be processed.  
🔹 `poll()` tells us which sockets **have activity** (e.g., incoming connections or data to read).  

#### **How Are Active Events Processed?**
```cpp
for (size_t i = 0; i < pollfds.size() && numEvents > 0; i++) {
    if (pollfds[i].revents & POLLIN) { 
        numEvents--; 
        if (isServerSocket(pollfds[i].fd)) { 
            // New connection on a server socket
            int serverIdx = getServerIndex(pollfds[i].fd);
            acceptConnection(serverIdx);
        } else {
            // Incoming data on a client socket
            handleClient(i);
        }
    }
}
```
- `pollfds[i].revents & POLLIN` → Checks if a socket has data to read (i.e., an **active event**).  
- `numEvents--` → Decreases the count because an event was processed.  
- The loop **stops early** if all events (`numEvents`) are processed.  

---

## **What Are Server Sockets?**
A **server socket** is a special socket that **listens for incoming connections** from clients.  
Your server **does not** communicate directly through this socket; instead, it waits for a client to connect and then creates a **client socket** for communication.

### **How Does a Server Socket Work?**
1. **Create a server socket**
   ```cpp
   int socketFd = socket(AF_INET, SOCK_STREAM, 0);
   ```
2. **Bind it to an IP and port**
   ```cpp
   bind(socketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
   ```
3. **Start listening for connections**
   ```cpp
   listen(socketFd, 10);
   ```
4. **When a client connects, accept it**
   ```cpp
   int clientFd = accept(socketFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
   ```

### **How Is a Server Socket Processed in `run()`?**
```cpp
if (isServerSocket(pollfds[i].fd)) { 
    int serverIdx = getServerIndex(pollfds[i].fd);
    acceptConnection(serverIdx);
}
```
- If the socket is a **server socket**, it means a **new client** is trying to connect.
- `acceptConnection(serverIdx)` **accepts the connection** and creates a **new client socket**.

---

## **What Are Client Sockets?**
A **client socket** is created when a server **accepts a connection** from a client.  
This socket is used **for actual communication** (sending and receiving HTTP requests & responses).  

### **How Does a Client Socket Work?**
1. **Created when a client connects**
   ```cpp
   int clientFd = accept(serverSocketFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
   ```
2. **Added to the `poll()` list** for monitoring.
3. **Receives data from the client**
   ```cpp
   recv(clientFd, buffer, BUFFER_SIZE, 0);
   ```
4. **Processes the HTTP request and sends a response**
   ```cpp
   send(clientFd, response.c_str(), response.size(), 0);
   ```
5. **Closes after the response is sent**
   ```cpp
   close(clientFd);
   ```

### **How Is a Client Socket Processed in `run()`?**
```cpp
else {
    handleClient(i);
}
```
- If it's **not a server socket**, it means it's a **client socket** with **incoming data**.
- `handleClient(i)` reads and processes the **HTTP request** and **sends a response**.

---

## **Final Summary**
1️⃣ **Server sockets** → Always **listening** for new connections. When a client connects, a **client socket** is created.  
2️⃣ **Client sockets** → Used for **actual communication** with clients (sending/receiving data).  
3️⃣ **poll() checks for active sockets** → If a socket has data, it gets processed.  
4️⃣ **Active events** are handled based on **whether they come from a server socket (new connection) or client socket (HTTP request).**  

This design allows your server to **handle multiple clients simultaneously** without blocking! 🚀

