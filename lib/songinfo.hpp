#pragma once

#include "memoryinputstream.hpp"
#include "memoryoutputstream.hpp"

struct SongInfo
{
    uint32_t sampleRate;
    
    uint16_t channels;

    unsigned length;

    unsigned size;

    void write(MemoryOutputStream &oStream)
    {
        oStream.write(&sampleRate, sizeof(uint32_t));
        oStream.write(&channels, sizeof(uint16_t));
        oStream.write(length);
        oStream.write(size);
    }

    void read(MemoryInputStream &iStream)
    {
        iStream.read(&sampleRate, sizeof(uint32_t));
        iStream.read(&channels, sizeof(uint16_t));
        iStream.read(length);
        iStream.read(size);
    }
};
