#!/usr/bin/env python3

import http.cookies
import os

cookie = http.cookies.SimpleCookie()
cookie["user"] = "John"
cookie["user"]["path"] = "/"
cookie["user"]["max-age"] = 3600

print("Content-Type: text/html")
print(cookie.output())
print()

pookie</title>
</head>
<body>
    <h1>Python CGI Cookie Example</h1>
    <p>Cookie 'user' is set to 'John'.</p>
</body>
</html>
""")