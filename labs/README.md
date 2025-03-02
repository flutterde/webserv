# Webserv Project Work Division

## Overview
The Webserv project involves creating an HTTP server in C++ 98. To ensure efficient collaboration among the three students, the work has been divided into three main parts: **Configuration Parsing**, **Core Server Functionality**, and **HTTP Request Handling**.

---

## Student 1: Configuration Parsing

### Responsibilities:
- Parse the configuration file provided as input.
- Implement functionality to read and interpret the configuration settings.
- Ensure the server can handle multiple servers, ports, and routes as defined in the configuration file.
- Validate the configuration file for correctness and completeness.

### Specific Tasks:
- Read the configuration file (either from the command-line argument or default path).
- Parse and store the following settings:
  - Port and host for each server.
  - Server names (optional).
  - Default error pages.
  - Client body size limits.
  - Routes with rules/configuration:
    - Accepted HTTP methods.
    - HTTP redirections.
    - File/directory roots.
    - Directory listing options.
    - Default files for directory requests.
    - CGI execution based on file extensions.
    - File upload configurations.
- Provide a data structure (e.g., a class or struct) to represent the parsed configuration.
- Handle edge cases, such as missing or invalid configuration entries.

### Deliverables:
- A module or class responsible for parsing the configuration file.
- Unit tests to validate the parsing logic.

---

## Student 2: Core Server Functionality

### Responsibilities:
- Set up the core server infrastructure.
- Implement non-blocking I/O using `poll()`, `epoll()`, `kqueue()`, or `select()`.
- Manage client connections and server operations.

### Specific Tasks:
- Initialize the server sockets for all configured ports.
- Use non-blocking file descriptors for all I/O operations.
- Implement the event loop using `poll()` (or equivalent) to handle read/write events simultaneously.
- Accept incoming client connections without blocking.
- Manage multiple client connections efficiently.
- Ensure the server remains responsive under high load.
- Implement basic error handling for socket operations and resource exhaustion.

### Deliverables:
- A module or class responsible for managing server sockets and client connections.
- Implementation of the event loop.
- Stress tests to verify the server's ability to handle multiple clients.

---

## Student 3: HTTP Request Handling

### Responsibilities:
- Handle HTTP requests and responses.
- Implement the required HTTP methods (`GET`, `POST`, `DELETE`).
- Ensure accurate HTTP response status codes and headers.

### Specific Tasks:
- Parse incoming HTTP requests.
- Support the following HTTP methods:
  - `GET`: Serve static files or directories.
  - `POST`: Handle file uploads and form submissions.
  - `DELETE`: Remove specified resources.
- Generate appropriate HTTP responses, including:
  - Status codes (e.g., 200 OK, 404 Not Found, 500 Internal Server Error).
  - Headers (e.g., `Content-Type`, `Content-Length`).
  - Body content for static files or error pages.
- Implement CGI support for dynamic content generation:
  - Execute CGI scripts based on file extensions.
  - Handle chunked requests and unchunk them before passing to the CGI.
  - Process CGI output and return it to the client.
- Ensure compatibility with the chosen web browser and NGINX behavior.

### Deliverables:
- A module or class responsible for handling HTTP requests and generating responses.
- Unit tests for HTTP request parsing and response generation.
- Demonstration of CGI functionality with at least one example (e.g., PHP-CGI or Python).

---

## Collaboration Guidelines

- All students should regularly communicate to ensure seamless integration of their components.
- Use version control (e.g., Git) to manage code changes and avoid conflicts.
- Agree on a common interface or API between modules to facilitate interaction.
- Test the entire system together to ensure all parts work harmoniously.

---

## Final Notes

Each student is responsible for their assigned part, but collaboration is key to the success of the project. Regular meetings and code reviews will help identify and resolve issues early. Remember to stress-test the server and ensure it meets all requirements outlined in the project description.

Good luck!