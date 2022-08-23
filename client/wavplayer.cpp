#include "wavplayer.hpp"

#include <chrono>

void WavPlayer::eval(int err)
{
    if(err < 0 && !m_inError)
    {
        m_inError = true;
        m_error = std::string(snd_strerror(err));
    }
}

void WavPlayer::doWork()
{
    int chunkSize = 1024;
    int frameSize = chunkSize / m_channels;
    while(m_isRunning)
    {
        if(m_isPlaying && m_currentData != nullptr)
        {
            if(m_currentSample < m_currentData->size)
            {
                snd_pcm_writei(m_sndDevice, m_currentData->data + m_currentSample, frameSize);
                m_currentSample += chunkSize;
            }
            else
            {
                if(!m_dataQueue.empty())
                {
                    if(m_currentData != nullptr)
                    {
                        delete m_currentData;
                        m_currentData = nullptr;
                    }
                    m_currentData = m_dataQueue.front();
                    m_currentSample = 0;
                    m_dataQueue.pop();
                }
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(17));
            if(!m_dataQueue.empty())
            {
                if(m_currentData != nullptr)
                {
                    delete m_currentData;
                    m_currentData = nullptr;
                }
                m_currentData = m_dataQueue.front();
                m_currentSample = 0;
                m_dataQueue.pop();
            }
        }
    }
}

WavPlayer::WavPlayer(unsigned sampleRate, unsigned channels)
    :m_periodSize{0}, m_sampleRate{sampleRate}, m_channels{channels}, m_currentSample{0}, m_inError{false}, m_isPlaying{false}, m_isRunning{true}, m_error{""}, m_currentData{nullptr}, m_worker{nullptr}
{
    eval(snd_pcm_open(&m_sndDevice, "default", SND_PCM_STREAM_PLAYBACK, 0));
    snd_pcm_hw_params_t *hwParams;
    eval(snd_pcm_hw_params_malloc(&hwParams));
    eval(snd_pcm_hw_params_any(m_sndDevice, hwParams));
    int direction = 0;
    eval(snd_pcm_hw_params_set_rate_near(m_sndDevice, hwParams, &m_sampleRate, &direction));
    eval(snd_pcm_hw_params_set_format(m_sndDevice, hwParams, SND_PCM_FORMAT_S16_LE));
    eval(snd_pcm_hw_params_set_access(m_sndDevice, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED));
    m_frames = 32;
    eval(snd_pcm_hw_params_set_period_size_near(m_sndDevice, hwParams, &m_frames, &direction));
    m_periodTime = 1024;
    eval(snd_pcm_hw_params_set_period_time_near(m_sndDevice, hwParams, &m_periodTime, &direction));
    eval(snd_pcm_hw_params_set_channels_near(m_sndDevice, hwParams, &m_channels));
    if(!m_inError)
    {
        eval(snd_pcm_hw_params(m_sndDevice, hwParams));
        snd_pcm_hw_params_get_period_size(hwParams, &m_periodSize, &direction);
        snd_pcm_hw_params_get_period_time(hwParams, &m_periodTime, &direction);
        m_worker = new std::thread(&WavPlayer::doWork, this);
    }
    snd_pcm_hw_params_free(hwParams);
}

WavPlayer::~WavPlayer()
{
    if(!m_inError)
    {
        m_isPlaying = false;
        snd_pcm_drain(m_sndDevice);
        snd_pcm_close(m_sndDevice);
    }
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
}

void WavPlayer::play(char *data, unsigned size)
{
    if(m_inError)
    {
        return;
    }
    WavData *w = new WavData(data, size);
    m_dataQueue.push(w);
}

void WavPlayer::resume()
{
    if(m_isPlaying)
    {
        return;
    }
    snd_pcm_prepare(m_sndDevice);
    m_isPlaying = true;
}

void WavPlayer::pause()
{
    if(!m_isPlaying)
    {
        return;
    }
    m_isPlaying = false;
    snd_pcm_drop(m_sndDevice);
}

void WavPlayer::stop()
{
    m_isRunning = false;
    pause();
}

unsigned WavPlayer::numberDataChunks() const
{
    return m_dataQueue.size();
}
