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
    // Location of the sprites in memory.
    //
    constexpr uint16_t kSpritesAddress = 0x0000;

    //
    // Size of a sprite in bytes
    //
    constexpr uint8_t kSpriteSize = 5U;

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

    using OpcodeType = uint16_t;

#pragma warning(disable : 4201)
    struct Opcode
    {
        union
        {
            struct
            {
                // Reversed for little endian.
                uint8_t nib1 : 4;
                uint8_t nib0 : 4;
            };
            uint8_t first_byte;
        };
        union
        {
            struct
            {
                // Reversed for little endian.
                uint8_t nib3 : 4;
                uint8_t nib2 : 4;

            };
            uint8_t second_byte;
        };

        std::string ToString() const
        {
            return std::format("{:#06x}", (first_byte << 8) | second_byte);
        }
    };

    //
    // Overload << so the opcode can be printed easily.
    //
    std::ostream& operator<<(std::ostream& os, Opcode const& arg)
    {
        os << arg.ToString();
        return os;
    }

    //
    // Instruction opcodes
    //
    enum class Instruction : OpcodeType
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
        kSetVxVy = 0x8000,
        kOrVxVy = 0x8001,
        kAndVxVy = 0x8002,
        kXorVxVy = 0x8003,
        kAddVxVy = 0x8004,
        kSubVxVy = 0x8005,
        kShrVxVy = 0x8006,
        kSubnVxVy = 0x8007,
        kShlVxVy = 0x800E,

        // 0x9 instructions
        kSkipNextInstructionIfXNotEqY = 0x9000,

        // 0xA instructions
        kSetIndexRegister = 0xA000,

        // 0xB instruction
        kJumpOffset = 0xB000,

        // 0xC instructions
        kRandom = 0xC000,

        // 0xD instructions
        kDraw = 0xD000,

        // 0xE instruction
        kSkipIfPressed = 0xE09E,
        kSkipIfNotPressed = 0xE0A1,

        // 0xF instructions
        kStoreDelayTimer = 0xF007,
        kStoreKeyPress = 0xF00A,
        kSetDelayTimer = 0xF015,
        kSetSoundTimer = 0xF018,
        kAddIVx = 0xF01E,
        kSetSpriteFromVx = 0xF029,
        kStoreBcdFromVx = 0xF033,
        kStoreRegisters = 0xF055,
        kSetRegisters = 0xF065,
    };
}
