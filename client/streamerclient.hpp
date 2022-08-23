#pragma once

#include <thread>
#include <vector>
#include <string>

#include "songinfo.hpp"
#include "tcpsocket.hpp"
#include "wavplayer.hpp"
#include "socketaddress.hpp"

class StreamerClient
{
private:
    bool m_isRunning;

    bool m_isPlaying;

    bool m_locked;

    TcpSocket *m_socket;

    WavPlayer *m_player;

    SongInfo *m_song;

    std::thread *m_worker;

    std::string m_fileName;

    unsigned m_bytesPlayed;

    void doWork();

public:
    StreamerClient(SocketAddress &serverAddr);

    StreamerClient(const StreamerClient &other) = delete;

    StreamerClient operator=(const StreamerClient &other) = delete;

    ~StreamerClient();

    void getSongs(std::vector<std::string> &files);

    void play(const std::string &fileName);

    void start();

    void stop();

    void pause();

    void resume();

    bool inError() const;

    const std::string &errorMsg() const;
};
