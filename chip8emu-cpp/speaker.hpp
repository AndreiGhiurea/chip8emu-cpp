#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include <atomic>

namespace chip8_emu
{
    class Speaker
    {
    public:
        Speaker()
        {
            if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
                std::cerr << "SDL_INIT_AUDIO failed: " << SDL_GetError() << std::endl;
                return;
            }

            SDL_AudioSpec want, have;
            SDL_zero(want);
            want.freq = 44100;
            want.format = AUDIO_S16SYS;
            want.channels = 1;
            want.samples = 2048;
            want.callback = AudioCallback;
            want.userdata = this;

            device_ = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
            if (device_ == 0)
            {
                std::cerr << "Failed to open audio: " << SDL_GetError() << std::endl;
            }
            else
            {
                SDL_PauseAudioDevice(device_, 0); // Start playing
            }
        }

        Speaker(const Speaker&) = delete;
        Speaker(Speaker&&) = delete;
        Speaker& operator=(const Speaker&) = delete;
        Speaker& operator=(Speaker&&) = delete;

        ~Speaker()
        {
            if (device_ != 0)
            {
                SDL_CloseAudioDevice(device_);
            }
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
        }

        void Play(bool play)
        {
            is_playing_ = play;
        }

    private:
        static void AudioCallback(void* userdata, Uint8* stream, int len)
        {
            Speaker* speaker = static_cast<Speaker*>(userdata);
            Sint16* buffer = reinterpret_cast<Sint16*>(stream);
            int length = len / 2;
            const int sample_rate = 44100;
            const int tone_hz = 440; // A4
            const int amplitude = 8000;
            const int period = sample_rate / tone_hz;
            const int half_period = period / 2;

            for (int i = 0; i < length; ++i)
            {
                if (speaker->is_playing_)
                {
                    buffer[i] = (speaker->sample_index_ < half_period) ? amplitude : -amplitude;
                    speaker->sample_index_ = (speaker->sample_index_ + 1) % period;
                }
                else
                {
                    buffer[i] = 0;
                }
            }
        }

        SDL_AudioDeviceID device_ = 0;
        std::atomic<bool> is_playing_ = false;
        int sample_index_ = 0;
    };
}
