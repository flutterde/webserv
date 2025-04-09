#!/usr/bin/bash

TIMEOUT_DURATION=4  # Set the timeout duration (in seconds)

run_with_timeout() {
    local cmd="$1"
    local timeout="$2"
    # Run the command in the background
    $cmd &
    local cmd_pid=$!
    # Wait for the command or timeout
    ( sleep $timeout && kill -9 $cmd_pid ) &  # Kill the process if it times out
    local timeout_pid=$!
    # Wait for the process to finish
    wait $cmd_pid
    local exit_status=$?
    # Clean up timeout handler
    kill $timeout_pid 2>/dev/null
    return $exit_status
}

echo "Test should work✅......."
run_with_timeout "../webserv ../configs/config.conf" $TIMEOUT_DURATION
if [ $? -eq 137 ]; then
    echo "Error: webserv test01 - 01 timed out"
elif [ $? -ne 0 ]; then
    echo "Error: webserv test01 - 01 failed"
else
    echo "webserv test01 - 01 success"
fi

echo "Test should failed❌......."
run_with_timeout "./webserv configs/con-fig.conf" $TIMEOUT_DURATION
if [ $? -eq 137 ]; then
    echo "Error: webserv test01 - 02 timed out"
elif [ $? -ne 0 ]; then
    echo "Error: webserv test01 - 02 failed"
else
    echo "webserv test01 - 02 success"
fi
