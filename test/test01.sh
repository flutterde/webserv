#!/usr/bin/bash

echo "Test should work✅......."
./webserv configs/config.conf 1> /dev/null
if [ $? -ne 0 ]; then
	echo "Error: webserv test01 - 01 failed"
else
	echo "webserv test01 - 01 success"
fi

echo "Test should failed❌......."
./webserv configs/con-fig.conf 1> /dev/null
if [ $? -ne 0 ]; then
	echo "Error: webserv test01 - 02 failed"
else
	echo "webserv test01 - 02 success"
fi
