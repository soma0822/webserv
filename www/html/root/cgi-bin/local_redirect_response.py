#!/usr/bin/python3

import cgitb
cgitb.enable()

print("Status: 303 See Other\r\n", end="")
print("Location: /cgi-bin/cgi_test.py\r\n\r\n", end="")
