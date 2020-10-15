CXX := g++
CFLAGS := -Wall -Wpedantic -std=c++2a

.PHONY: run

run:
	mkdir -p build
	$(CXX) test.cc $(CFLAGS) -o build/test
	./build/test
