CXX = g++
CXXFLAGS = -std=c++20 -Ichip8emu-cpp
TEST_SRC = tests/test_stack.cpp
TEST_EXE = tests/test_stack

all: test

test: $(TEST_EXE)
	./$(TEST_EXE)

$(TEST_EXE): $(TEST_SRC) chip8emu-cpp/stack.hpp chip8emu-cpp/constants.hpp
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(TEST_EXE)

clean:
	rm -f $(TEST_EXE)

.PHONY: all test clean
