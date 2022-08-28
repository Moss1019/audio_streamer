#pragma once

#include <queue>
#include <string>
#include <thread>

#include <inttypes.h>
#include <alsa/asoundlib.h>

#include "wavchunkdata.hpp"

class WavPlayer
{
private:
    unsigned m_sampleRate;

    unsigned m_channels;

    bool m_hasError;

    bool m_playing;

    bool m_running;

    std::string m_error;

    snd_pcm_t *m_sndDevice;

    std::thread *m_worker;

    uint16_t *m_audioData;

    unsigned m_audioDataSize;

    unsigned m_bytesPlayed;

    WavChunkData *m_currentData;

    std::queue<WavChunkData *> m_dataQueue;

    void eval(int code);

    void doWork();

public:
    WavPlayer(unsigned sampleRate, unsigned channels);

    WavPlayer(const WavPlayer &other) = delete;

    WavPlayer operator=(const WavPlayer &other) = delete;

    ~WavPlayer();

    bool hasError() const;

    const std::string &error() const;

    unsigned numberChunks() const;

    void play(uint16_t *buffer, unsigned size);

    void play(char *buffer, unsigned size);

    void resume();

    void pause();
};
