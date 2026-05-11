#pragma once
#include <algorithm>

#include <algorithm>
#include <array>
#include <format>
#include <stdexcept>

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
                    const auto it = std::find(uint_to_scancode_.begin(), uint_to_scancode_.end(), event.key.keysym.scancode);

                    if (it != uint_to_scancode_.end())
                    {
                        return static_cast<uint8_t>(std::distance(uint_to_scancode_.begin(), it));
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
    };
}
