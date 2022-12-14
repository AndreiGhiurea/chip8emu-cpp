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

        ~Stack() = default;

    private:
        uint16_t data_[kStackSize] = { 0x00 };
    };
}
