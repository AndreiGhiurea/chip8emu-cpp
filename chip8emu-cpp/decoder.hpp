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
            switch (opcode.nib0)
            {
            case 0x0:
                return DecodeNibble0X0(opcode);
            case 0x1:
                return Instruction::kJump;
            case 0x2:
                return Instruction::kCall;
            case 0x3:
                return Instruction::kSkipNextInstructionIfEq;
            case 0x4:
                return Instruction::kSkipNextInstructionIfNotEq;
            case 0x5:
                return Instruction::kSkipNextInstructionIfXEqY;
            case 0x6:
                return Instruction::kSetVxRegister;
            case 0x7:
                return Instruction::kAddToRegister;
            case 0x8:
                return DecodeNibble0X8(opcode);
            case 0x9:
                return Instruction::kSkipNextInstructionIfXNotEqY;
            case 0xA:
                return Instruction::kSetIndexRegister;
            case 0xB:
                return Instruction::kJumpOffset;
            case 0xC:
                return Instruction::kRandom;
            case 0xD:
                return Instruction::kDraw;
            case 0xE:
                return DecodeNibble0XE(opcode);
            case 0xF:
                return DecodeNibble0XF(opcode);
            default:
                throw std::runtime_error{ std::format("Unknown instruction: {}", opcode.ToString()) };
            }
        }

    private:
        static Instruction DecodeNibble0X0(const Opcode opcode)
        {
            switch (opcode.second_byte)
            {
            case 0xE0:
                return Instruction::kClearScreen;
            case 0xEE:
                return Instruction::kReturn;
            case 0x00:
                return Instruction::kSys;
            default:
                throw std::runtime_error{ std::format("Invalid instruction: {}", opcode.ToString()) };
            }
        }

        static Instruction DecodeNibble0X8(Opcode opcode)
        {
            switch (opcode.nib3)
            {
            case 0x0:
                return Instruction::kSetVxVy;
            case 0x1:
                return Instruction::kOrVxVy;
            case 0x2:
                return Instruction::kAndVxVy;
            case 0x3:
                return Instruction::kXorVxVy;
            case 0x4:
                return Instruction::kAddVxVy;
            case 0x5:
                return Instruction::kSubVxVy;
            case 0x6:
                return Instruction::kShrVxVy;
            case 0x7:
                return Instruction::kSubnVxVy;
            case 0xE:
                return Instruction::kShlVxVy;
            default:
                throw std::runtime_error{ std::format("Invalid instruction: {}", opcode.ToString()) };
            }
        }

        static Instruction DecodeNibble0XE(Opcode opcode)
        {
            switch (opcode.second_byte)
            {
            case 0x9E:
                return Instruction::kSkipIfPressed;
            case 0xA1:
                return Instruction::kSkipIfNotPressed;
            default:
                throw std::runtime_error{ std::format("Invalid instruction: {}", opcode.ToString()) };
            }
        }

        static Instruction DecodeNibble0XF(Opcode opcode)
        {
            switch (opcode.second_byte)
            {
            case 0x07:
                return Instruction::kStoreDelayTimer;
            case 0x0A:
                return Instruction::kStoreKeyPress;
            case 0x15:
                return Instruction::kSetDelayTimer;
            case 0x18:
                return Instruction::kSetSoundTimer;
            case 0x1E:
                return Instruction::kAddIVx;
            case 0x29:
                return Instruction::kSetSpriteFromVx;
            case 0x33:
                return Instruction::kStoreBcdFromVx;
            case 0x55:
                return Instruction::kStoreRegisters;
            case 0x65:
                return Instruction::kSetRegisters;
            default:
                throw std::runtime_error{ std::format("Invalid instruction: {}", opcode.ToString()) };
            }
        }
    };
}
