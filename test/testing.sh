#!/usr/bin/bash

PORT_1=8080
PORT_2=8081
PORT_3=8082

HOST="127.0.0.1"

NUMBER_OF_RETRIES=10000
SUCCESS=0
FAILED=0
TIMEOUT=0.1
# This script is used to run the tests for the project.


for ((i=0; i < $NUMBER_OF_RETRIES; i++))
do
	# USING CURL
	curl --max-time $TIMEOUT -s $HOST:$PORT_1 1>/dev/null
	if [ $? -ne 0 ]; then
		FAILED=$((FAILED + 1))
	else
		SUCCESS=$((SUCCESS + 1))
	fi
	curl --max-time $TIMEOUT -s $HOST:$PORT_2 1>/dev/null
	if [ $? -ne 0 ]; then
		FAILED=$((FAILED + 1))
	else
		SUCCESS=$((SUCCESS + 1))
	fi
	curl --max-time $TIMEOUT -s $HOST:$PORT_3 1>/dev/null
	if [ $? -ne 0 ]; then
		FAILED=$((FAILED + 1))
	else
		SUCCESS=$((SUCCESS + 1))
	fi
	echo "Test ($i) All services are up and running.."
	 echo "Success: $SUCCESS"
	 echo "Failed: $FAILED"
done
echo " =================== RESULTS ======================"

echo "Test ($i) All services are up and running.."
echo "Success: $SUCCESS"
echo "Failed: $FAILED"
echo "Total: $((SUCCESS + FAILED))"
echo "Success Rate: $((SUCCESS * 100 / (SUCCESS + FAILED)))%"
echo "Failed Rate: $((FAILED * 100 / (SUCCESS + FAILED)))%"
echo "Test completed."
