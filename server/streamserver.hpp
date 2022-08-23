#pragma once

#include "tcpserver.hpp"

class StreamServer
{
private:
    TcpServer *m_server;

    std::string m_audioFolder;

    onReceiveCallback m_onReceive;

public:
    StreamServer(const SocketAddress &address, onAcceptedCallback onAccepted);

    StreamServer(const StreamServer &other) = delete;

    StreamServer operator=(const StreamServer &other) = delete;

    ~StreamServer();

    void start();

    void stop();
};
