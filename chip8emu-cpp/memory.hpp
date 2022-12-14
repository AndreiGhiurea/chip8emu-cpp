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
                throw std::runtime_error(std::format("Can't access memory at offset {}, there's only {} bytes of memory", offset, kMemorySize));
            }

            return &data_[offset];
        }

        void Write(const std::vector<uint8_t>& bytes, const uint16_t offset = 0x00)
        {
            if (offset + bytes.size() > kMemorySize)
            {
                throw std::runtime_error(std::format("Can't write {} bytes from offset {}, there's only {} bytes of memory", bytes.size(), offset, kMemorySize));
            }

            std::memcpy(&data_[offset], bytes.data(), bytes.size());
        }

        Opcode FetchOpcode(uint16_t& pc) const
        {
            Opcode opcode = data_[pc];
            opcode = (opcode << 8) | data_[pc + 1];

            pc += 2;

            return opcode;
        }

    private:
        uint8_t data_[kMemorySize] = { 0x00 };
    };

}