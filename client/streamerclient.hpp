#pragma once

#include <vector>
#include <string>
#include <thread>

#include "wavplayer.hpp"
#include "tcpsocket.hpp"
#include "socketaddress.hpp"

class StreamerClient
{
private:
    TcpSocket *m_socket;

    WavPlayer *m_player;

    std::thread *m_worker;

    bool m_running;

    bool m_playing;

    uint32_t m_audioDataSize;

    std::string m_fileName;

    void doWork();

public:
    StreamerClient(const SocketAddress &remoteServer);

    StreamerClient(const StreamerClient &other) = delete;

    StreamerClient operator=(const StreamerClient &other) = delete;

    ~StreamerClient();

    void getSongs(std::vector<std::string> &files) const;

    uint32_t play(const std::string &fileName);

    void pause();

    void resume();

    bool inError() const;

    const std::string &errorMsg() const;
};
