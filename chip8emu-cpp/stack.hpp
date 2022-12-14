#pragma once

#include <cstdint>

#include "constants.hpp"

namespace chip8_emu
{
    class Stack
    {
    public:
        Stack() = default;

        Stack(const Stack&) = delete;
        Stack(Stack&&) = delete;

        Stack& operator=(const Stack&) = delete;
        Stack& operator=(Stack&&) = delete;

        void push(const uint16_t value)
        {
            if (sp_ >= kStackSize)
            {
                throw std::runtime_error{ "Stack is full" };
            }

            data_[sp_] = value;
            sp_++;
        }

        uint16_t pop(void)
        {
            if (sp_ == 0)
            {
                throw std::runtime_error{ "Stack is empty" };
            }

            sp_--;
            return data_[sp_];
        }

        ~Stack() = default;

    private:
        uint8_t sp_ = 0;
        uint16_t data_[kStackSize] = { 0x00 };
    };
}
