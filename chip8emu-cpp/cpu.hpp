#pragma once

#include <format>
#include <iostream>
#include <vector>

#include "display.hpp"
#include "memory.hpp"
#include "constants.hpp"
#include "decoder.hpp"
#include "stack.hpp"

namespace chip8_emu
{
    class Cpu
    {
    public:
        Cpu() = default;

        Cpu(const Cpu&) = delete;
        Cpu(Cpu&&) = delete;

        Cpu& operator=(const Cpu&) = delete;
        Cpu& operator=(Cpu&&) = delete;

        ~Cpu() = default;

        void Run()
        {
            while (true)
            {
                const Opcode opcode = memory_.FetchOpcode(registers_.pc);
                std::cout << std::hex << opcode << std::endl;

                const auto instruction = Decoder::Decode(opcode);
                Emulate(instruction, opcode);
            }
        }

        void Load(const std::vector<uint8_t>& bytes)
        {
            memory_.Write(bytes, 0x200);

            //
            // CHIP-8 programs are usually loaded at 0x200
            //
            registers_.pc = 0x200;
        }

    private:
        void Emulate(const Instruction instruction, const Opcode opcode)
        {
            switch (instruction)
            {
            case Instruction::kClearScreen:
                display_.Clear();
                display_.Refresh();
                break;
            case Instruction::kJump:
                registers_.pc = opcode & (~kFirstNibbleMask);
                break;
            case Instruction::kSetVxRegister:
            {
                const uint8_t reg_nr = (opcode & kSecondNibbleMask) >> 8;
                registers_.v[reg_nr] = (opcode & kLastByteMask);
                break;
            } 
            case Instruction::kAddToRegister:
            {
                const uint8_t reg_nr = (opcode & kSecondNibbleMask) >> 8;
                registers_.v[reg_nr] += (opcode & kLastByteMask);
                break;
            }
            case Instruction::kSetIndexRegister:
                registers_.index = opcode & (~kFirstNibbleMask);
                break;
            case Instruction::kDraw:
            {
                const uint8_t x = (opcode & kSecondNibbleMask) >> 8;
                const uint8_t y = (opcode & kThirdNibbleMask) >> 4;
                const uint8_t n = (opcode & kFourthNibbleMask);

                const uint8_t* sprite = static_cast<uint8_t*>(memory_.Data(registers_.index));
                display_.Draw(registers_.v[x], registers_.v[y], sprite, n);
                display_.Refresh();
                break;
            }
            default:
                throw std::runtime_error(std::format("Instruction {} not implemented", static_cast<Opcode>(instruction)));
            }
        }
        
        Registers registers_{ 0x00 };
        Stack stack_;
        Memory memory_;
        Display display_;
    };

}
