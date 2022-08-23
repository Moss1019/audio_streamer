#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <unistd.h>
#endif

#include <string>

#include "types.hpp"
#include "socketaddress.hpp"
#include "memoryoutputstream.hpp"

class TcpSocket
{
private:
    socktype m_sock;

    bool m_inError;

    bool m_isConnected;

    std::string m_errorMsg;

public:
    TcpSocket();

    TcpSocket(const SocketAddress &remote);

    TcpSocket(socktype sock);

    ~TcpSocket();

    bool inError() const;

    const std::string &errorMsg() const;

    int sendData(const MemoryOutputStream &stream);

    int receiveData(MemoryOutputStream &stream);
};

