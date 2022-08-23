#pragma once

#include <alsa/asoundlib.h>

#include <queue>
#include <string>
#include <thread>
#include <cstring>

#include <inttypes.h>

struct WavData 
{
    unsigned size;

    uint16_t *data;

    WavData(): size{0}, data{nullptr} {}

    WavData(char *data, unsigned size)
    {
        this->size = size / sizeof(uint16_t);
        this->data = new uint16_t[this->size];
        std::memcpy(this->data, data, size);
    }

    ~WavData()
    {
        if(data != nullptr)
        {
            delete[] data;
            data = nullptr;
        }
    }
};

class WavPlayer 
{
private:
    snd_pcm_t *m_sndDevice;

    snd_pcm_uframes_t m_frames;

    snd_pcm_uframes_t m_periodSize;

    unsigned m_periodTime;

    unsigned m_sampleRate;

    unsigned m_channels;

    unsigned m_currentSample;

    bool m_inError;

    bool m_isPlaying;

    bool m_isRunning;

    std::string m_error;

    std::queue<WavData *> m_dataQueue;

    WavData *m_currentData;

    std::thread *m_worker;

    void eval(int err);

    void doWork();

public:
    WavPlayer(unsigned sampleRate, unsigned channels);

    WavPlayer(const WavPlayer &other) = delete;

    WavPlayer operator=(const WavPlayer &other) = delete;

    ~WavPlayer();

    void play(char *data, unsigned size);

    void resume();

    void pause();

    void stop();

    unsigned numberDataChunks() const;
};