#pragma once

#include <vector>

#include "constants.hpp"

namespace chip8_emu
{
    class Memory
    {
    public:
        Memory() = default;

        Memory(const Memory&) = delete;
        Memory(Memory&&) = delete;

        Memory& operator=(const Memory&) = delete;
        Memory& operator=(Memory&&) = delete;

        ~Memory() = default;

        void* Data(const uint16_t offset = 0x00)
        {
            if (offset >= kMemorySize)
            {
                throw std::runtime_error{ std::format("Can't access memory at offset {}, there's only {} bytes of memory", offset, kMemorySize) };
            }

            return &data_[offset];
        }

        void Write(const std::vector<uint8_t>& bytes, const uint16_t offset = 0x00)
        {
            if (offset + bytes.size() > kMemorySize)
            {
                throw std::runtime_error{ std::format("Can't write {} bytes from offset {}, there's only {} bytes of memory", bytes.size(), offset, kMemorySize) };
            }

            std::memcpy(&data_[offset], bytes.data(), bytes.size());
        }

        void Write(const uint8_t byte, const uint16_t address)
        {
            if (address >= kMemorySize)
            {
                throw std::runtime_error{ "Memory out of bounds" };
            }

            data_[address] = byte;
        }

        uint8_t Read(const uint16_t address) const
        {
            if (address >= kMemorySize)
            {
                throw std::runtime_error{ "Memory out of bounds" };
            }

            return data_[address];
        }

        Opcode FetchOpcode(uint16_t& pc) const
        {
            Opcode opcode;

            opcode.first_byte = data_[pc];
            opcode.second_byte = data_[pc + 1];

            pc += 2;

            return opcode;
        }

    private:
        uint8_t data_[kMemorySize] =
        {
            //
            // Preload the sprites in memory at location 0x000
            //
            0xF0, 0x90, 0x90, 0x90, 0xF0, // "0"
            0x20, 0x60, 0x20, 0x20, 0x70, // "1"
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // "2"
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // "3"
            0x90, 0x90, 0xF0, 0x10, 0x10, // "4"
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // "5"
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // "6"
            0xF0, 0x10, 0x20, 0x40, 0x40, // "7"
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // "8"
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // "9"
            0xF0, 0x90, 0xF0, 0x90, 0x90, // "A"
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // "B"
            0xF0, 0x80, 0x80, 0x80, 0xF0, // "C"
            0xE0, 0x90, 0x90, 0x90, 0xE0, // "D"
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // "E"
            0xF0, 0x80, 0xF0, 0x80, 0x80, // "F"
        };
    };

}