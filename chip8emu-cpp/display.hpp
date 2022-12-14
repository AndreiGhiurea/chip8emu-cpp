#pragma once

#include <cstdint>
#include <bitset>

#include "SDL.h"
#undef main

#include "constants.hpp"

namespace chip8_emu
{
    class Display
    {
    public:
        Display()
        {
            //
            // Spin up a thread to render the window.
            //
            is_stopping_ = false;
            render_thread_ = std::thread([this] { RenderWindow(); });
        }

        Display(const Display&) = delete;
        Display(Display&&) = delete;

        Display& operator=(const Display&) = delete;
        Display& operator=(Display&&) = delete;

        ~Display()
        {
            is_stopping_ = true;
            render_thread_.join();
        }

        void Clear()
        {
            std::memset(data_, 0x00, kHorizontalDisplaySize * kVerticalDisplaySize);
        }

        bool Draw(const uint16_t x, uint16_t y, const uint8_t *sprite, const uint8_t sprite_size)
        {
            bool pixel_turned_off = false;

            //
            // Make sure coords are within bounds.
            // If we reach the bottom screen border stop drawing.
            //
            y = y % kVerticalDisplaySize;
            for (auto i = 0U; i < sprite_size && y < kVerticalDisplaySize; i++)
            {
                //
                // Use a local x coord because we need to come back at
                // the start of the row for each new pixel.
                //
                auto local_x = x % kHorizontalDisplaySize;

                //
                // This byte describes which pixels must switched on/off
                // starting on positions x,y. Each bytes describes a line of 8 pixels.
                // The next byte describes the next 8 pixels underneath and so on.
                // While pixels is != 0 that means we still have pixels we need to turn on (bits set).
                // Stop turning on pixels if we hit the right screen border.
                //
                auto pixels = sprite[i];
                while (pixels && local_x < kHorizontalDisplaySize)
                {
                    //
                    // If the left-most bit is set we have to toggle the current pixel on x,y.
                    //
                    if (pixels & 0x80)
                    {
                        auto& display_pixel = data_[local_x][y];

                        //
                        // If the pixel is already on, that means we're turning it off.
                        // We have to remember this and set the VF register to 1.
                        //
                        if (display_pixel)
                        {
                            pixel_turned_off = true;
                        }

                        //
                        // Toggle the pixel.
                        //
                        display_pixel ^= 0xFF;
                    }

                    //
                    // Shift to test the next pixel.
                    // Increment local_x to skip to the next column.
                    //
                    pixels <<= 1;
                    local_x++;
                }

                //
                // Finished drawing 8 inline pixels.
                // Move over to the next line.
                //
                y++;
            }

            return pixel_turned_off;
        }

        void Refresh()
        {
            if (window_ == nullptr)
            {
                //
                // Wait until the window is ready.
                //
                std::unique_lock window_lock{ windows_mtx_ };
                window_is_ready_.wait(window_lock);
            }

            //
            // Get window surface
            //
            const auto screen_surface = SDL_GetWindowSurface(window_);

            SDL_Rect rectangle;
            for (uint8_t x = 0U; x < kHorizontalDisplaySize; x++)
            {
                for (uint8_t y = 0U; y < kVerticalDisplaySize; y++)
                {
                    //
                    // Turn pixels white or black.
                    //
                    const uint8_t color =
                        data_[x][y] ? 0xFF : 0x00;

                    //
                    // Draw the pixel on the screen surface.
                    // In case we used a window resolution bigger than the display size
                    // we need to treat multiple pixels as a single one, so we set h and w > 1
                    //
                    rectangle.x = x * kWindowZoomFactor;
                    rectangle.y = y * kWindowZoomFactor;
                    rectangle.h = kWindowZoomFactor;
                    rectangle.w = kWindowZoomFactor;

                    SDL_FillRect(screen_surface, &rectangle, SDL_MapRGB(screen_surface->format, color, color, color));
                }
            }

            //
            // Update the surface
            //
            SDL_UpdateWindowSurface(window_);
        }

    private:
        void RenderWindow()
        {
            //
            // Initialize SDL
            // 
            SDL_Init(SDL_INIT_VIDEO);

            //
            // Create an application window
            //
            window_ = SDL_CreateWindow(
                "CHIP-8 Emu",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                kHorizontalWindowSize,
                kVerticalWindowSize,
                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
            );

            if (window_ == nullptr) {
                throw std::runtime_error("SDL window couldn't be created");
            }

            //
            // Notify that the window is ready.
            //
            window_is_ready_.notify_one();

            SDL_Event e;
            while (!is_stopping_) 
            {
                SDL_PollEvent(&e);
            }

            //
            // Close and destroy the window
            //
            SDL_DestroyWindow(window_);
            window_ = nullptr;

            //
            // Clean up
            //
            SDL_Quit();
        }

        //
        // Windows renderer thread.
        //
        std::thread render_thread_;

        //
        // Signals the rendering thread if the emulator is stopping.
        //
        std::atomic_bool is_stopping_ = false;

        //
        // Used for waiting until the window is ready for use.
        //
        std::mutex windows_mtx_;
        std::condition_variable window_is_ready_;

        //
        // CHIP-8 internal display data
        //
        uint8_t data_[kHorizontalDisplaySize][kVerticalDisplaySize] = {0x00};

        //
        // SDL Display windows used to show data to the user
        //
        SDL_Window* window_ = nullptr;
    };
}
