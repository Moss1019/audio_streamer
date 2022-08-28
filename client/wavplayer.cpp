#include "wavplayer.hpp"

#include <chrono>

void WavPlayer::eval(int code)
{
    if(code < 0)
    {
        m_hasError = true;
        m_error = std::string(snd_strerror(code));
    }
}

void WavPlayer::doWork()
{
    while(m_running)
    {
        if(m_playing)
        {
            if(m_currentData != nullptr)
            {
                if(m_bytesPlayed < m_currentData->audioChunkSize)
                {
                    if(snd_pcm_state(m_sndDevice) != SND_PCM_STATE_RUNNING)
                    {
                        snd_pcm_prepare(m_sndDevice);
                    }
                    snd_pcm_writei(m_sndDevice, m_currentData->audioChunk, m_currentData->audioChunkSize / m_channels);
                    m_bytesPlayed += m_currentData->audioChunkSize;
                    std::this_thread::sleep_for(std::chrono::milliseconds(400));
                }
                else
                {
                    delete m_currentData;
                    m_currentData = nullptr;
                }
            }
            else
            {
                if(!m_dataQueue.empty())
                {
                    m_currentData = m_dataQueue.front();
                    m_dataQueue.pop();
                    m_bytesPlayed = 0;
                }
            }
        }
    }
}

WavPlayer::WavPlayer(unsigned sampleRate, unsigned channels)
    :m_sampleRate{sampleRate}, m_channels{channels}, m_hasError{false}, m_error{}, m_sndDevice{nullptr},
    m_worker{nullptr}, m_audioData{nullptr}, m_currentData{nullptr}
{
    snd_pcm_open(&m_sndDevice, "default", SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_hw_params_t *params;
    snd_pcm_hw_params_malloc(&params);
    eval(snd_pcm_hw_params_any(m_sndDevice, params));
    int dir;
    eval(snd_pcm_hw_params_set_rate_near(m_sndDevice, params, &m_sampleRate, &dir));
    eval(snd_pcm_hw_params_set_access(m_sndDevice, params, SND_PCM_ACCESS_RW_INTERLEAVED));
    eval(snd_pcm_hw_params_set_channels_near(m_sndDevice, params, &m_channels));
    eval(snd_pcm_hw_params_set_format(m_sndDevice, params, SND_PCM_FORMAT_S16_LE));
    snd_pcm_hw_params(m_sndDevice, params);
    snd_pcm_hw_params_free(params);
    m_running = true;
    m_worker = new std::thread(&WavPlayer::doWork, this);
}

WavPlayer::~WavPlayer()
{
    m_running = false;
    m_playing = false;
    if(m_worker != nullptr)
    {
        m_worker->join();
        delete m_worker;
        m_worker = nullptr;
    }
    while(!m_dataQueue.empty())
    {
        delete m_dataQueue.front();
        m_dataQueue.pop();
    }
    if(m_sndDevice != nullptr)
    {
        snd_pcm_drain(m_sndDevice);
        snd_pcm_close(m_sndDevice);
    }
}

bool WavPlayer::hasError() const
{
    return m_hasError;
}

const std::string &WavPlayer::error() const 
{
    return m_error;
}

unsigned WavPlayer::numberChunks() const
{
    return m_dataQueue.size();
}

void WavPlayer::play(uint16_t *buffer, unsigned size)
{
    if(snd_pcm_state(m_sndDevice) != SND_PCM_STATE_RUNNING)
    {
        snd_pcm_prepare(m_sndDevice);
    }
    snd_pcm_writei(m_sndDevice, buffer, size / m_channels);
}

void WavPlayer::play(char *buffer, unsigned size)
{
    m_dataQueue.push(new WavChunkData(buffer, size));
}

void WavPlayer::resume()
{
    m_playing = true;
}

void WavPlayer::pause()
{
    m_playing = false;
    snd_pcm_drop(m_sndDevice);
}
