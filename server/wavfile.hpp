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

    WavFile(const char *fileData)
        :audioData{nullptr}
    {
        std::memcpy(fileType, fileData, 4);
        std::memcpy(&fileSize, fileData + 4, 4);
        std::memcpy(fileHeader, fileData + 8, 4);
        std::memcpy(&formatLength, fileData + 12, 4);
        std::memcpy(formatMarker, fileData + 16, 4);
        std::memcpy(&formatType, fileData + 20, 2);
        std::memcpy(&channels, fileData + 22, 2);
        std::memcpy(&sampleRate, fileData + 24, 4);
        std::memcpy(&clockRate, fileData + 28, 4);
        std::memcpy(&bitsPerFrame, fileData + 32, 2);
        std::memcpy(&bitsPerSample, fileData + 34, 2);
        std::memcpy(dataMarker, fileData + 36, 4);
        std::memcpy(&audioDataSize, fileData + 40, 4);
        audioData = new char[audioDataSize];
        std::memcpy(audioData, fileData + 44, audioDataSize);
    }

    WavFile(const WavFile &other) = delete;

    ~WavFile()
    {
        if(audioData != nullptr)
        {
            delete[] audioData;
            audioData = nullptr;
        }
    }
};

