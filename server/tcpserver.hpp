#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include <thread>
#include <vector>

#include "types.hpp"
#include "socketaddress.hpp"

typedef void (*onAcceptedCallback)(void);

class TcpServer
{
private:
    bool m_isRunning;

    bool m_inError;

    socktype m_sock;

    std::thread *m_acceptThread;

    std::string m_errorMsg;

    std::vector<TcpClient *> m_clients;

    onReceiveCallback m_onReceive;

	onAcceptedCallback m_onAccepted;

    void acceptCallback();

public:
    TcpServer(const SocketAddress &host, onReceiveCallback onReceive, onAcceptedCallback onAccepted);

    TcpServer(const TcpServer &other) = delete;

    TcpServer operator=(const TcpServer &other) = delete;

    ~TcpServer();

    bool inError() const;

    const std::string &errorMsg() const;

    void start();

    void stop();
};
