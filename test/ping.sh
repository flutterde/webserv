#!/usr/bin/bash

HOST="172.217.17.14"
PORT=8090
TOTAL_TESTS=20
SUCCESS=0
TIMEOUT=0.5  # Set timeout in seconds
echo "-------------- ✅❇️❌ -----------------"
./webserv configs/config.conf &

for ((i=0; i < $TOTAL_TESTS; i++))
do
    # Run curl with a timeout of $TIMEOUT seconds
    curl --max-time $TIMEOUT -s $HOST:$PORT 1>/dev/null
    # Check if curl was successful
    if [ $? -eq 0 ]; then
        SUCCESS=$((SUCCESS + 1))
    fi
done

echo "==> ✅success $SUCCESS/$TOTAL_TESTS"
echo "==> ❌failed  $((TOTAL_TESTS - SUCCESS))"
