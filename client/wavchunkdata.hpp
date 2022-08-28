#pragma once

#include <cstring>

#include <inttypes.h>

#include "memoryinputstream.hpp"
#include "memoryoutputstream.hpp"

struct WavChunkData
{
    uint16_t *audioChunk;

    unsigned audioChunkSize;

    WavChunkData()
        :audioChunk{nullptr}, audioChunkSize{0}
    {}

    WavChunkData(const char *data, unsigned size)
        :audioChunk{nullptr}, audioChunkSize{0}
    {
        audioChunkSize = size / sizeof(uint16_t);
        audioChunk = new uint16_t[audioChunkSize];
        std::memcpy(audioChunk, data, size);
    }

    WavChunkData(const WavChunkData &other) = delete;

    ~WavChunkData()
    {
        if(audioChunk != nullptr)
        {
            delete[] audioChunk;
            audioChunk = nullptr;
        }
    }

    void write(MemoryOutputStream &stream)
    {
        stream.write(audioChunkSize);
        stream.write(audioChunk, audioChunkSize);
    }

    void read(MemoryInputStream &stream)
    {
        stream.read(audioChunkSize);
        audioChunk = new uint16_t[audioChunkSize];
        stream.read(audioChunk, audioChunkSize);
    }
};
