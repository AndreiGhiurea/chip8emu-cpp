#pragma once

#include <array>
#include <format>
#include <stdexcept>
#include <unordered_map>

#include "SDL.h"

namespace chip8_emu
{
    class Keyboard
    {
    public:
        Keyboard() = default;

        Keyboard(const Keyboard&) = delete;
        Keyboard(Keyboard&&) = delete;

        Keyboard& operator=(const Keyboard&) = delete;
        Keyboard& operator=(Keyboard&&) = delete;

        ~Keyboard() = default;

        uint8_t WaitForKeyPress()
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_KEYDOWN)
                {
                    const auto it = scancode_to_uint_.find(event.key.keysym.scancode);

                    if (it != scancode_to_uint_.cend())
                    {
                        return it->second;
                    }
                }
            }

            throw std::runtime_error{ "Unexpected exit from WaitForKeyPress function" };
        }

        bool IsKeyPressed(const uint8_t key) const
        {
            if (key >= uint_to_scancode_.size())
            {
                throw std::runtime_error{ std::format("Could find {} key", key) };
            }

            const auto scancode = uint_to_scancode_[key];
            const auto state = SDL_GetKeyboardState(nullptr);
            if (state[scancode])
            {
                return true;
            }

            return false;
        }
    private:
        static constexpr std::array<SDL_Scancode, 16> uint_to_scancode_
        {
            SDL_SCANCODE_X, // 0x0
            SDL_SCANCODE_1, // 0x1
            SDL_SCANCODE_2, // 0x2
            SDL_SCANCODE_3, // 0x3
            SDL_SCANCODE_Q, // 0x4
            SDL_SCANCODE_W, // 0x5
            SDL_SCANCODE_E, // 0x6
            SDL_SCANCODE_A, // 0x7
            SDL_SCANCODE_S, // 0x8
            SDL_SCANCODE_D, // 0x9
            SDL_SCANCODE_Z, // 0xA
            SDL_SCANCODE_C, // 0xB
            SDL_SCANCODE_4, // 0xC
            SDL_SCANCODE_R, // 0xD
            SDL_SCANCODE_F, // 0xE
            SDL_SCANCODE_V, // 0xF
        };

        const std::unordered_map<SDL_Scancode, uint8_t> scancode_to_uint_
        {
            {uint_to_scancode_[0x1], (uint8_t)0x1},
            {uint_to_scancode_[0x2], (uint8_t)0x2},
            {uint_to_scancode_[0x3], (uint8_t)0x3},
            {uint_to_scancode_[0xC], (uint8_t)0xC},
            {uint_to_scancode_[0x4], (uint8_t)0x4},
            {uint_to_scancode_[0x5], (uint8_t)0x5},
            {uint_to_scancode_[0x6], (uint8_t)0x6},
            {uint_to_scancode_[0xD], (uint8_t)0xD},
            {uint_to_scancode_[0x7], (uint8_t)0x7},
            {uint_to_scancode_[0x8], (uint8_t)0x8},
            {uint_to_scancode_[0x9], (uint8_t)0x9},
            {uint_to_scancode_[0xE], (uint8_t)0xE},
            {uint_to_scancode_[0xA], (uint8_t)0xA},
            {uint_to_scancode_[0x0], (uint8_t)0x0},
            {uint_to_scancode_[0xB], (uint8_t)0xB},
            {uint_to_scancode_[0xF], (uint8_t)0xF},
        };
    };
}
