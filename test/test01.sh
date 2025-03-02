#!/usr/bin/bash

set -e

./webserv configs/config.conf 1> /dev/null

if [ $? -ne 0 ]; then
	echo "Error: webserv test01 - 01 failed"
	exit 1
else
	echo "webserv test01 - 01 success"
	exit 0
fi