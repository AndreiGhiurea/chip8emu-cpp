#pragma once

#include <csignal>
#include <format>
#include <iostream>
#include <vector>

#include "display.hpp"
#include "memory.hpp"
#include "constants.hpp"
#include "decoder.hpp"
#include "keyboard.hpp"
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
            timers_thread_ = std::thread([this]() { Timer(); });

            while (true)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));

                const Opcode opcode = memory_.FetchOpcode(registers_.pc);
                std::cout << std::hex << opcode << std::endl;

                const auto instruction = Decoder::Decode(opcode);
                Emulate(instruction, opcode);
            }

            timers_thread_.join();
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
        void Timer()
        {
            while (true)
            {
                if (registers_.delay_timer != 0)
                {
                    registers_.delay_timer -= 1;
                }

                if (registers_.sound_timer != 0)
                {
                    // Sound the buzzer I guess
                    registers_.sound_timer -= 1;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }
        }

        void Emulate(const Instruction instruction, const Opcode opcode)
        {
            switch (instruction)
            {
            case Instruction::kReturn:
                registers_.pc = stack_.pop();
                break;
            case Instruction::kClearScreen:
                display_.Clear();
                display_.Refresh();
                break;
            case Instruction::kJump:
                registers_.pc = (opcode.nib1 << 8) | opcode.second_byte;
                break;
            case Instruction::kCall:
                stack_.push(registers_.pc);
                registers_.pc = (opcode.nib1 << 8) | opcode.second_byte;
            case Instruction::kSetVxRegister:
            {
                registers_.v[opcode.nib1] = opcode.second_byte;
                break;
            }
            case Instruction::kAddToRegister:
            {
                registers_.v[opcode.nib1] += opcode.second_byte;
                break;
            }
            case Instruction::kSetVxVy:
                registers_.v[opcode.nib1] = registers_.v[opcode.nib2];
                break;
            case Instruction::kOrVxVy:
                registers_.v[opcode.nib1] |= registers_.v[opcode.nib2];
                break;
            case Instruction::kAndVxVy:
                registers_.v[opcode.nib1] &= registers_.v[opcode.nib2];
                break;
            case Instruction::kXorVxVy:
                registers_.v[opcode.nib1] ^= registers_.v[opcode.nib2];
                break;
            case Instruction::kAddVxVy:
                if ((registers_.v[opcode.nib1] + registers_.v[opcode.nib2]) > 255)
                {
                    registers_.v[0xF] = 0x1;
                }
                else
                {
                    registers_.v[0xF] = 0x0;
                }

                registers_.v[opcode.nib1] += registers_.v[opcode.nib2];
                break;
            case Instruction::kSubVxVy:
                if (registers_.v[opcode.nib1] >= registers_.v[opcode.nib2])
                {
                    registers_.v[0xF] = 0x1;
                }
                else
                {
                    registers_.v[0xF] = 0x0;
                }

                registers_.v[opcode.nib1] = registers_.v[opcode.nib1] - registers_.v[opcode.nib2];
                break;
            case Instruction::kSubnVxVy:
                if (registers_.v[opcode.nib2] >= registers_.v[opcode.nib1])
                {
                    registers_.v[0xF] = 0x1;
                }
                else
                {
                    registers_.v[0xF] = 0x0;
                }

                registers_.v[opcode.nib1] = registers_.v[opcode.nib2] - registers_.v[opcode.nib1];
                break;
            case Instruction::kShrVxVy:
                if (registers_.v[opcode.nib1] & 0x1)
                {
                    registers_.v[0xF] = 0x1;
                }
                else
                {
                    registers_.v[0xF] = 0x0;
                }

                registers_.v[opcode.nib1] >>= 1;
                break;
            case Instruction::kShlVxVy:
                if (registers_.v[opcode.nib1] & 0x80)
                {
                    registers_.v[0xF] = 0x1;
                }
                else
                {
                    registers_.v[0xF] = 0x0;
                }

                registers_.v[opcode.nib1] <<= 1;
                break;
            case Instruction::kSetIndexRegister:
                registers_.index = (opcode.nib1 << 8) | opcode.second_byte;
                break;
            case Instruction::kJumpOffset:
                registers_.pc = ((opcode.nib1 << 8) | opcode.second_byte) + registers_.v[0];
                break;
            case Instruction::kRandom:
                registers_.v[opcode.nib1] = std::rand() & opcode.second_byte;
                break;
            case Instruction::kDraw:
            {
                const uint8_t x = opcode.nib1;
                const uint8_t y = opcode.nib2;
                const uint8_t n = opcode.nib3;
                
                const uint8_t* sprite = static_cast<uint8_t*>(memory_.Data(registers_.index));
                const auto pixel_turned_off = display_.Draw(registers_.v[x], registers_.v[y], sprite, n);
                display_.Refresh();

                registers_.v[0xF] = pixel_turned_off ? 0x1 : 0x0;

                break;
            }
            case Instruction::kSkipNextInstructionIfEq:
                if (registers_.v[opcode.nib1] == opcode.second_byte)
                {
                    registers_.pc += 2;
                }
                break;
            case Instruction::kSkipNextInstructionIfNotEq:
                if (registers_.v[opcode.nib1] != opcode.second_byte)
                {
                    registers_.pc += 2;
                }
                break;
            case Instruction::kSkipNextInstructionIfXEqY:
                if (registers_.v[opcode.nib1] == registers_.v[opcode.nib2])
                {
                    registers_.pc += 2;
                }
                break;
            case Instruction::kSkipNextInstructionIfXNotEqY:
                if (registers_.v[opcode.nib1] != registers_.v[opcode.nib2])
                {
                    registers_.pc += 2;
                }
                break;
            case Instruction::kSkipIfPressed:
            {
                const auto key = registers_.v[opcode.nib1];
                if (keyboard_.IsKeyPressed(key))
                {
                    registers_.pc += 2;
                }
                break;
            }
            case Instruction::kSkipIfNotPressed:
            {
                const auto key = registers_.v[opcode.nib1];
                if (!keyboard_.IsKeyPressed(key))
                {
                    registers_.pc += 2;
                }
                break;
            }
            case Instruction::kStoreDelayTimer:
                registers_.v[opcode.nib1] = registers_.delay_timer;
                break;
            case Instruction::kStoreKeyPress:
                registers_.v[opcode.nib1] = keyboard_.WaitForKeyPress();
                break;
            case Instruction::kSetDelayTimer:
                registers_.delay_timer = registers_.v[opcode.nib1];
                break;
            case Instruction::kSetSoundTimer:
                registers_.sound_timer = registers_.v[opcode.nib1];
                break;
            case Instruction::kAddIVx:
                registers_.index = registers_.index + registers_.v[opcode.nib1];
            case Instruction::kSetSpriteFromVx:
                registers_.index = kSpritesAddress + kSpriteSize * opcode.nib1;
                break;
            case Instruction::kStoreBcdFromVx:
            {
                uint8_t dec = 0;
                const auto address = registers_.index;

                dec = registers_.v[opcode.nib1] / 100;
                memory_.Write(dec, address);

                dec = registers_.v[opcode.nib1] / 10 % 10;
                memory_.Write(dec, address + 1);

                dec = registers_.v[opcode.nib1] % 10;
                memory_.Write(dec, address + 2);
                break;
            }
            case Instruction::kStoreRegisters:
            {
                const auto address = registers_.index;
                for (uint8_t i = 0; i <= opcode.nib1; i++)
                {
                    memory_.Write(registers_.v[i], address + i);
                }
                break;
            }
            case Instruction::kSetRegisters:
            {
                const auto address = registers_.index;
                for (uint8_t i = 0; i <= opcode.nib1; i++)
                {
                    registers_.v[i] = memory_.Read(address + i);
                }
                break;
            }
            default:
                throw std::runtime_error{ std::format("Instruction {} not implemented", opcode.ToString()) };
            }
        }

        Registers registers_{ 0x00 };
        Stack stack_;
        Memory memory_;
        Display display_;
        Keyboard keyboard_;

        std::thread timers_thread_;
    };

}
