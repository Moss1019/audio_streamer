#include "socketaddress.hpp"

#ifdef _WIN32
#include <WinSock2.h>
#endif

#include <cstring>

SocketAddress::SocketAddress()
    :m_addr{}
{
}

SocketAddress::SocketAddress(const std::string &ipAddress, unsigned short port)
{
    sockaddr_in host;
    std::memset(&host, 0, sizeof(sockaddr_in));
#ifdef _WIN32
    wchar_t wIpAddress[17];
    unsigned numChars = ipAddress.length();
    size_t returnValue;
    mbstowcs_s(&returnValue, wIpAddress, ipAddress.c_str(), ipAddress.length());
    wIpAddress[numChars] = '\0';
    InetPton(AF_INET, wIpAddress, &host.sin_addr);
#else
    inet_pton(AF_INET, ipAddress.c_str(), &host.sin_addr);
#endif
    host.sin_family = AF_INET;
    host.sin_port = htons(port);
    m_addr = *(reinterpret_cast<sockaddr *>(&host));
}

SocketAddress::SocketAddress(const sockaddr &remote)
{
    std::memcpy(&m_addr, &remote, sizeof(sockaddr));
}

const sockaddr *SocketAddress::constAddress() const
{
    return &m_addr;
}

unsigned SocketAddress::addressLen() const
{
    return sizeof(sockaddr);
}

const std::string SocketAddress::ipAddress()
{
    char buffer[16];
    inet_ntop(AF_INET, &(reinterpret_cast<sockaddr_in *>(&m_addr)->sin_addr), buffer, 16);
    return std::string(buffer);
}
