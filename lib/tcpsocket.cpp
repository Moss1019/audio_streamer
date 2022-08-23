#include "tcpsocket.hpp"

#include "tcpmessage.hpp"
#include "memoryinputstream.hpp"

socktype m_sock;

bool m_inError;

bool m_isConnected;

std::string m_errorMsg;

TcpSocket::TcpSocket()
    :m_sock{0}, m_inError{false}, m_isConnected{false}, m_errorMsg{}
{
}

TcpSocket::TcpSocket(const SocketAddress &remote)
    :TcpSocket{}
{
    m_sock = socket(PF_INET, SOCK_STREAM, 0);
    int res = connect(m_sock, remote.constAddress(), remote.addressLen());
    if (res < 0)
    {
        m_inError = true;
        m_errorMsg = "Could not connect to remote server";
    }
}

TcpSocket::TcpSocket(socktype sock)
    :TcpSocket{}
{
    m_sock = sock;
}

TcpSocket::~TcpSocket()
{
#ifdef _WIN32
    shutdown(m_sock, SD_BOTH);
    closesocket(m_sock);
#else
    shutdown(m_sock, SHUT_RDWR);
    close(m_sock);
#endif
    m_inError = true;
    m_errorMsg = "Socket connection closed";
}

bool TcpSocket::inError() const
{
    return m_inError;
}

const std::string &TcpSocket::errorMsg() const
{
    return m_errorMsg;
}

int TcpSocket::sendData(const MemoryOutputStream &stream)
{
    if (m_inError)
    {
        return -1;
    }
    int bytesSent = send(m_sock, stream.getBufferPtr(), stream.getLength(), 0);
    return bytesSent;
}

int TcpSocket::receiveData(MemoryOutputStream &stream)
{
    if (m_inError)
    {
        return -1;
    }
    unsigned bufferSize = 1024;
    char *buffer = new char[bufferSize];
    bool isReceiving = true;
    int bytesRead = 0;
    int totalRead = 0;
    do
    {
        bytesRead = recv(m_sock, buffer, bufferSize, 0);
        if (bytesRead == -1)
        {
            m_errorMsg = "Remote socket closed";
            m_inError = true;
        }
        else
        {
            totalRead += bytesRead;
            stream.write(buffer, bytesRead);
        }
        if (bytesRead < bufferSize)
        {
            isReceiving = false;
        } 
    } while (isReceiving);
    delete[] buffer;
    return totalRead;
}
