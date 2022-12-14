#pragma once

#include <cstdint>

namespace chip8_emu
{
    //
    // CHIP-8 memory size, 4KB
    //
    constexpr uint16_t kMemorySize = 0x1000;

    //
    // CHIP-8 stack size, 16 16-bit addresses
    //
    constexpr uint8_t kStackSize = 0x10;

    //
    // Horizontal display size
    //
    constexpr uint8_t kHorizontalDisplaySize = 64U;

    //
    // Vertical display size
    //
    constexpr uint8_t kVerticalDisplaySize = 32U;

    //
    // Window zoom factor. By how much to upscale the pixels.
    //
    constexpr uint8_t kWindowZoomFactor = 10;

    //
    // Display window horizontal size
    //
    constexpr uint16_t kHorizontalWindowSize = kHorizontalDisplaySize * kWindowZoomFactor;

    //
    // Display window vertical size
    //
    constexpr uint16_t kVerticalWindowSize = kVerticalDisplaySize * kWindowZoomFactor;

    //
    // Number of general purpose registers.
    // There are 16 of them. V0 -> VF.
    //
    constexpr uint8_t kNumberOfGeneralRegisters = 0x10;

    //
    // CHIP-8 registers
    //
    struct Registers
    {
        // Program counter
        uint16_t pc;

        // Index register. Points a locations in memory
        uint16_t index;

        // Delay timer register.
        // Decremented at a rate of 60hz until it reaches 0.
        uint8_t delay_timer;

        // Sound timer register.
        // Functions like the delay timer. Gives off a beeping sound as long as it's not 0.
        uint8_t sound_timer;

        // General purpose register.
        uint8_t v[kNumberOfGeneralRegisters];
    };

    //
    // Instruction masks
    //
    constexpr uint16_t kFirstNibbleMask = 0xF000;
    constexpr uint16_t kSecondNibbleMask = 0x0F00;
    constexpr uint16_t kThirdNibbleMask = 0x00F0;
    constexpr uint16_t kFourthNibbleMask = 0x000F;

    constexpr uint16_t kFirstByteMask = kFirstNibbleMask | kSecondNibbleMask;
    constexpr uint16_t kLastByteMask = kThirdNibbleMask | kFourthNibbleMask;

    using Opcode = uint16_t;

    //
    // Instruction opcodes
    //
    enum class Instruction : Opcode
    {
        // 0x0 instructions
        kSys = 0x0000,
        kClearScreen = 0x00E0,
        kReturn = 0x00EE,

        // 0x1 instructions
        kJump = 0x1000,

        // 0x2 instructions
        kCall = 0x2000,

        // 0x3 instructions
        kSkipNextInstructionIfEq = 0x3000,

        // 0x4 instructions
        kSkipNextInstructionIfNotEq = 0x4000,

        // 0x5 instructions
        kSkipNextInstructionIfXEqY = 0x5000,

        // 0x6 instructions
        kSetVxRegister = 0x6000,

        // 0x7 instructions
        kAddToRegister = 0x7000,

        // 0x8 instructions

        // 0xA instructions
        kSetIndexRegister = 0xA000,

        // 0xD instructions
        kDraw = 0xD000,
    };
}
