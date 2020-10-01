CXX := g++
CFLAGS := -Wall -Wpedantic -std=c++17

run:
	$(CXX) test.cc $(CFLAGS) -o test
	./test
