#pragma once

#include <stdint.h>

#include <cstring>

struct WavFile
{
    uint32_t sampleRate;
    
    uint16_t channels;

    unsigned length;

    unsigned size;

    char *frames{nullptr};

    ~WavFile()
    {
        if(frames != nullptr)
        {
            delete[] frames;
            frames = nullptr;
        }
    }

    void read(char *buffer)
    {
        std::memcpy(&channels, buffer + 22, 2);
        std::memcpy(&sampleRate, buffer + 24, 2);
        std::memcpy(&size, buffer + 40, 4);
        length = size;
        frames = new char[length];
        std::memcpy(frames, buffer + 44, size);
    }
};
