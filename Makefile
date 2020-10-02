CXX := g++
CFLAGS := -Wall -Wpedantic -std=c++17

.PHONY: run

run:
	$(CXX) test.cc $(CFLAGS) -o test
	./test
