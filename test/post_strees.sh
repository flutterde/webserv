TESTS_COUNT=10

for i in $(seq 1 $TESTS_COUNT); do
	echo "Test $i"
	bash post_testing.sh > "post_test_$i.log" &
done
