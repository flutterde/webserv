#!/usr/bin/env python3
import cgi
import cgitb
import os
from http import cookies
import json
import hashlib
import time

cgitb.enable()

SESSION_FILE = "/tmp/sessions.json"

def load_sessions():
    if not os.path.exists(SESSION_FILE):
        return {}
    with open(SESSION_FILE, 'r') as f:
        return json.load(f)

def save_sessions(sessions):
    with open(SESSION_FILE, 'w') as f:
        json.dump(sessions, f)

def generate_session_id():
    return hashlib.sha256(str(time.time()).encode()).hexdigest()[:16]

print("Content-type: text/html")

sessions = load_sessions()
cookie = cookies.SimpleCookie()
cookie.load(os.environ.get('HTTP_COOKIE', ''))

if 'session_id' in cookie:
    session_id = cookie['session_id'].value
    session_data = sessions.get(session_id, {})
else:
    session_id = generate_session_id()
    session_data = {}
    print(f"Set-Cookie: session_id={session_id}; Path=/")

form = cgi.FieldStorage()
if 'color' in form:
    session_data['color'] = form.getvalue('color')
    sessions[session_id] = session_data
    save_sessions(sessions)

current_color = session_data.get('color', '#000000')

html = f"""
<!DOCTYPE html>
<html>
<head>
    <title>Session Example</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            padding: 20px;
            color: {current_color};
        }}
        form {{ margin-top: 20px; }}
    </style>
</head>
<body>
    <h1>Session Color Picker</h1>
    <p>Current session ID: {session_id}</p>
    <p>Your selected color: {current_color}</p>
    
    <form method="POST">
        <label for="color">Choose a color:</label>
        <input type="color" id="color" name="color" value="{current_color}">
        <button type="submit">Save Color</button>
    </form>
    
    <p>This color will persist across page reloads.</p>
</body>
</html>
"""

print(html)
