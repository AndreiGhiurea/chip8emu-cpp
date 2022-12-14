#pragma once

#include <format>
#include <stdexcept>

#include "constants.hpp"

namespace chip8_emu
{
    class Decoder
    {
    public:
        static Instruction Decode(Opcode opcode)
        {
            switch (opcode & kFirstNibbleMask)
            {
            case 0x0000:
                return DecodeNibble0X0(opcode);
            case 0x1000:
                return Instruction::kJump;
            case 0x2000:
                return Instruction::kCall;
            case 0x3000:
                return Instruction::kSkipNextInstructionIfEq;
            case 0x4000:
                return Instruction::kSkipNextInstructionIfNotEq;
            case 0x5000:
                return Instruction::kSkipNextInstructionIfXEqY;
            case 0x6000:
                return Instruction::kSetVxRegister;
            case 0x7000:
                return Instruction::kAddToRegister;
            case 0x8000:
                return DecodeNibble0X8(opcode);
            case 0x9000:
                return Instruction::kClearScreen;
            case 0xA000:
                return Instruction::kSetIndexRegister;
            case 0xB000:
                return Instruction::kClearScreen;
            case 0xC000:
                return Instruction::kClearScreen;
            case 0xD000:
                return Instruction::kDraw;
            case 0xE000:
                return DecodeNibble0XE(opcode);
            case 0xF000:
                return DecodeNibble0XF(opcode);
            default:
                throw std::runtime_error(std::format("Unknown instruction: {}", opcode));
            }
        }

    private:
        static Instruction DecodeNibble0X0(const Opcode opcode)
        {
            switch (opcode & kLastByteMask)
            {
            case 0x00E0:
                return Instruction::kClearScreen;
            case 0x00EE:
                return Instruction::kReturn;
            default:
                return Instruction::kSys;
            }
        }

        static Instruction DecodeNibble0X8(Opcode opcode)
        {
            throw std::runtime_error(std::format("Instruction not implemented: {}", opcode));
        }

        static Instruction DecodeNibble0XE(uint16_t opcode)
        {
            throw std::runtime_error(std::format("Instruction not implemented: {}", opcode));
        }

        static Instruction DecodeNibble0XF(uint16_t opcode)
        {
            throw std::runtime_error(std::format("Instruction not implemented: {}", opcode));
        }
    };
}
