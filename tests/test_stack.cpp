#include <iostream>
#include <cassert>
#include <stdexcept>
#include "stack.hpp"

void test_stack_empty_pop() {
    chip8_emu::Stack stack;
    try {
        stack.pop();
        assert(false && "Should have thrown std::runtime_error");
    } catch (const std::runtime_error& e) {
        std::string message = e.what();
        assert(message == "Stack is empty");
        std::cout << "test_stack_empty_pop passed\n";
    }
}

void test_stack_push_pop() {
    chip8_emu::Stack stack;
    stack.push(0x123);
    stack.push(0x456);
    assert(stack.pop() == 0x456);
    assert(stack.pop() == 0x123);
    std::cout << "test_stack_push_pop passed\n";
}

void test_stack_full_push() {
    chip8_emu::Stack stack;
    for (int i = 0; i < chip8_emu::kStackSize; ++i) {
        stack.push(i);
    }
    try {
        stack.push(0x999);
        assert(false && "Should have thrown std::runtime_error");
    } catch (const std::runtime_error& e) {
        std::string message = e.what();
        assert(message == "Stack is full");
        std::cout << "test_stack_full_push passed\n";
    }
}

int main() {
    try {
        test_stack_empty_pop();
        test_stack_push_pop();
        test_stack_full_push();
        std::cout << "All Stack tests passed!\n";
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
