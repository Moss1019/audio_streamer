#pragma once

#include <cstring>

#include <inttypes.h>

struct WavFile
{
    char fileType[4];

    uint32_t fileSize;

    char fileHeader[4];

    char formatMarker[4];

    uint32_t formatLength;

    uint16_t formatType;

    uint16_t channels;

    uint32_t sampleRate;

    uint32_t clockRate;

    uint16_t bitsPerFrame;

    uint16_t bitsPerSample;

    char dataMarker[4];

    uint32_t audioDataSize;

    char *audioData;

    WavFile(const char *fileData);

    WavFile(const WavFile &other) = delete;

    ~WavFile();
};

