#!/usr/bin/env python3
import os

# Tell the browser this is plain text (not HTML)

# Print all env variables, one per line
for key, value in os.environ.items():
    print(f"{key}: {value}<br>")  # \n adds a newline