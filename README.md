# Webserv

A C++98 HTTP web server project, for the **42 school** curriculum.

---

## 🔍 Project Summary

**Goal:**  
Build a fully functional HTTP server from scratch using C++98.

- **Executable:** `./webserv [configuration file]`
- **Main Protocol:** HTTP (HyperText Transfer Protocol)
- **Testable with:** Real web browsers, telnet, and compared against NGINX

---

## 🧱 Key Features

**Mandatory:**
- Non-blocking I/O using `poll()` 
- Serve static files (HTML, images, etc.)
- Handle HTTP methods: `GET`, `POST`, `DELETE`
- File upload support
- CGI execution support (e.g., PHP, Python scripts)
- Multiple server blocks with virtual hosting
- Multiple listening ports

**Configuration file** (inspired by NGINX) can specify:
- Server names
- Root directories
- Allowed methods
- Directory listing
- Max body size
- Default files
- HTTP redirection

---

## ⚠️ Constraints

- Use only allowed syscalls (`recv`, `send`, `accept`, `poll`, etc.)
- No external or Boost libraries
- Handle client disconnections and errors gracefully (never crash)
- Remain non-blocking at all times
- Do not `execve` another server
- Only **one** `poll()` (or equivalent) loop for all I/O

---

## 🌟 Bonus Features (Optional)

- Cookie and session handling
- Multiple CGI support

---

## 🐳 Docker Support

A `Dockerfile` is included to simplify building and running the server in a containerized environment.

**To build and run with Docker:**
```bash
make up
```
*(Adjust the port as needed, and map configuration files/volumes if required.)*

---

## 📁 Submission & Build

- Includes a `Makefile` with rules: `all`, `clean`, `fclean`, `re`, `$(NAME)`
- This project is maintained in a Git repository

**To build:**
```bash
make
```

**To run:**
```bash
./webserv [configuration file]
```

---

## 📝 Evaluation Notes

- Peer evaluation will test all mandatory features and resilience
- Server must be testable with web browsers, telnet, and compared to NGINX behavior

---

## 📄 License

Project for educational purposes as part of the 42 school curriculum.

---
