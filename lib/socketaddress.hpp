#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include <string>

class SocketAddress
{
private:
    sockaddr m_addr;

public:
    SocketAddress();

    SocketAddress operator=(const SocketAddress &other) = delete;

    SocketAddress(const std::string &ipAddress, unsigned short port);

    SocketAddress(const sockaddr &remote);

    const sockaddr *constAddress() const;

    unsigned addressLen() const;

    const std::string ipAddress();
};

