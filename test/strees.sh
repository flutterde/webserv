TESTS_COUNT=10

for i in $(seq 1 $TESTS_COUNT); do
	echo "Test $i"
	bash testing.sh > "test_$i.log" &
done
