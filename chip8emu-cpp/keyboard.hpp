#pragma once

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
            const auto find_scancode = [&key](const auto& item)
            {
                return item.second == key;
            };

            const auto it = std::find_if(scancode_to_uint_.cbegin(), scancode_to_uint_.cend(), find_scancode);
            if (it == scancode_to_uint_.cend())
            {
                throw std::runtime_error{ std::format("Could find {} key", key) };
            }

            const auto scancode = it->first;
            const auto state = SDL_GetKeyboardState(nullptr);
            if (state[scancode])
            {
                return true;
            }

            return false;
        }
    private:
        const std::unordered_map<SDL_Scancode, uint8_t> scancode_to_uint_
        {
            {SDL_SCANCODE_1, (uint8_t)0x1},
            {SDL_SCANCODE_2, (uint8_t)0x2},
            {SDL_SCANCODE_3, (uint8_t)0x3},
            {SDL_SCANCODE_4, (uint8_t)0xC},
            {SDL_SCANCODE_Q, (uint8_t)0x4},
            {SDL_SCANCODE_W, (uint8_t)0x5},
            {SDL_SCANCODE_E, (uint8_t)0x6},
            {SDL_SCANCODE_R, (uint8_t)0xD},
            {SDL_SCANCODE_A, (uint8_t)0x7},
            {SDL_SCANCODE_S, (uint8_t)0x8},
            {SDL_SCANCODE_D, (uint8_t)0x9},
            {SDL_SCANCODE_F, (uint8_t)0xE},
            {SDL_SCANCODE_Z, (uint8_t)0xA},
            {SDL_SCANCODE_X, (uint8_t)0x0},
            {SDL_SCANCODE_C, (uint8_t)0xB},
            {SDL_SCANCODE_V, (uint8_t)0xF},
        };
    };
}
