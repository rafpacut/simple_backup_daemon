build: src/main.cpp
	g++ src/main.cpp -o build/main.out

test_simple: build
	test/test_driver/./basic_test.sh