#!/usr/bin/env python3

import http.cookies
import os

# Create a cookie
cookie = http.cookies.SimpleCookie()
cookie["user"] = "John"
cookie["user"]["path"] = "/"     # Cookie available site-wide
cookie["user"]["max-age"] = 3600 # Expires in 1 hour

# Print headers
print("Content-Type: text/html")
print(cookie.output())  # This sets the cookie in the browser
print()

# Print simple HTML
print("""
<!DOCTYPE html>
<html>
<head>
    <title>Set Cookie</title>
</head>
<body>
    <h1>Python CGI Cookie Example</h1>
    <p>Cookie 'user' is set to 'John'.</p>
</body>
</html>
""")