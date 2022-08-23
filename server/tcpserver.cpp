
#include "tcpserver.hpp"

#ifdef _WIN32

#else
#include <unistd.h>
#endif

#include <cstring>
#include <iostream>

#include "tcpclient.hpp"
#include "tcpsocket.hpp"
#include "tcpmessage.hpp"

void TcpServer::acceptCallback()
{
    while (m_isRunning)
    {
        sockaddr client;
        std::memset(&client, 0, sizeof(sockaddr));
        sockaddrLen clientSize = sizeof(sockaddr);
        socktype clientSock = accept(m_sock, &client, &clientSize);
        if (m_sock != -1)
        {
            if (clientSock != -1)
            {
                TcpClient *newClient = new TcpClient(clientSock, m_onReceive);
                m_clients.push_back(newClient);
                newClient->start();
                if(m_onAccepted != nullptr)
                {
                    m_onAccepted();
                }
            }
        }
        else
        {
            m_isRunning = false;
            m_inError = true;
            m_errorMsg = "Server socket in invalid state";
        }
    }
}

TcpServer::TcpServer(const SocketAddress &host, onReceiveCallback onReceive, onAcceptedCallback onAccepted)
    :m_isRunning{false}, m_inError{false}, m_acceptThread{nullptr}, m_onReceive{ onReceive }, m_onAccepted{onAccepted}
{
    m_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (bind(m_sock, host.constAddress(), host.addressLen()) < 0)
    {
        m_inError = true;
        m_errorMsg = "Could not bind to ip address";
    }
    if (listen(m_sock, 3) < 0)
    {
        m_inError = true;
        m_errorMsg = "Could not listen on socket";
    }
    m_isRunning = !m_inError;
}

TcpServer::~TcpServer()
{
    stop();
    if (m_acceptThread != nullptr)
    {
        m_acceptThread->join();
        delete m_acceptThread;
        m_acceptThread = nullptr;
    }
    for (const auto &c : m_clients)
    {
        c->stop();
        delete c;
    }
}

bool TcpServer::inError() const
{
    return m_inError;
}

const std::string &TcpServer::errorMsg() const
{
    return m_errorMsg;
}

void TcpServer::start()
{
    m_acceptThread = new std::thread(&TcpServer::acceptCallback, this);
}

void TcpServer::stop()
{
    m_isRunning = false;
#ifdef _WIN32
    shutdown(m_sock, SD_BOTH);
    closesocket(m_sock);
#else
    shutdown(m_sock, SHUT_RDWR);
    close(m_sock);
#endif
}
