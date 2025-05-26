#!/usr/bin/env python3
import cgi
import cgitb
cgitb.enable()

print("Content-type: text/html")

form = cgi.FieldStorage()
cookie_key = form.getvalue('cookieKey', '')
cookie_value = form.getvalue('cookieValue', '')

html = """
<!DOCTYPE html>
<html>
<head>
    <title>Cookie Setter</title>
    <style>
        /* Same CSS as above */
    </style>
</head>
<body>
    <h1>Set Cookie</h1>
"""

if cookie_key and cookie_value:
    print(f"Set-Cookie: {cookie_key}={cookie_value}; Path=/")
    html += f'<div class="result success">Cookie set successfully: {cookie_key}={cookie_value}</div>'
elif cookie_key or cookie_value:
    html += '<div class="result error">Please provide both cookie name and value</div>'

html += """
    <form method="POST" action="">
        <div class="form-group">
            <label for="cookieKey">Cookie Name:</label>
            <input type="text" id="cookieKey" name="cookieKey" required>
        </div>
        <div class="form-group">
            <label for="cookieValue">Cookie Value:</label>
            <input type="text" id="cookieValue" name="cookieValue" required>
        </div>
        <button type="submit">Set Cookie</button>
    </form>
</body>
</html>
"""

print(html)