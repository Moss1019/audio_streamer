#include "streamerclient.hpp"

#include <chrono>
#include <cstring>

#include <iostream>

#include "tcpmessage.hpp"

void StreamerClient::doWork()
{
    unsigned bytesPlayed;
    unsigned chunkSize = 44100 * 2 * 2 * 0.5;
    MemoryOutputStream out;
    while(m_running)
    {
        if(m_playing)
        {
            if(bytesPlayed < m_audioDataSize && m_player->numberChunks() < 1)
            {
                TcpMessage message;
                out.empty();
                out.write(bytesPlayed);
                out.write(chunkSize);
                unsigned fileNameSize = m_fileName.size();
                out.write(fileNameSize);
                out.write(m_fileName.c_str(), fileNameSize);
                message.messageType = 2003;
                message.dataLength = out.getBufferLength();
                message.data = new char[message.dataLength];
                std::memcpy(message.data, out.getBufferPtr(), message.dataLength);
                out.empty();
                message.write(out);
                int sent = m_socket->sendData(out);
                out.empty();
                int received = m_socket->receiveData(out);
                MemoryInputStream in(out.getBufferPtr(), out.getBufferLength());
                message.read(in);
                m_player->play(message.data, message.dataLength);
                bytesPlayed += message.dataLength;
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
        }
    }
}

StreamerClient::StreamerClient(const SocketAddress &remoteServer)
    :m_socket{nullptr}, m_player{nullptr}, m_worker{nullptr}, m_running{false}, m_playing{false}
{
    m_socket = new TcpSocket(remoteServer);
    m_player = new WavPlayer(44100, 2);
    m_running = true;
    m_worker = new std::thread(&StreamerClient::doWork, this);
}

StreamerClient::~StreamerClient()
{
    m_running = false;
    m_playing = false;
    if(m_worker != nullptr)
    {
        m_worker->join();
        delete m_worker;
        m_worker = nullptr;
    }
    if(m_socket != nullptr)
    {
        delete m_socket;
        m_socket = nullptr;
    }
    if(m_player != nullptr)
    {
        delete m_player;
        m_player = nullptr;
    }
}

void StreamerClient::getSongs(std::vector<std::string> &files) const
{
    TcpMessage message;
    message.messageType = 2001;
    MemoryOutputStream out;
    message.write(out);
    int sent = m_socket->sendData(out);
    out.empty();
    int received = m_socket->receiveData(out);
    MemoryInputStream in(out.getBufferPtr(), out.getBufferLength());
    message.read(in);
    in.setBuffer(message.data, message.dataLength);
    unsigned numberFiles;
    in.read(numberFiles);
    files.clear();
    for(unsigned i = 0; i < numberFiles; ++i)
    {
        unsigned fileNameLength;
        in.read(fileNameLength);
        char *fileNameBuffer = new char[fileNameLength];
        in.read(fileNameBuffer, fileNameLength);
        files.push_back(std::string(fileNameBuffer, fileNameLength));
        delete[] fileNameBuffer;
    }
}

uint32_t StreamerClient::play(const std::string &fileName) 
{
    TcpMessage message;
    message.messageType = 2002;
    message.dataLength = fileName.size();
    message.data = new char[message.dataLength];
    std::memcpy(message.data, fileName.c_str(), message.dataLength);
    MemoryOutputStream out;
    message.write(out);
    int sent = m_socket->sendData(out);
    out.empty();
    int received = m_socket->receiveData(out);
    MemoryInputStream in(out.getBufferPtr(), out.getBufferLength());
    message.read(in);
    in.setBuffer(message.data, message.dataLength);
    uint32_t audioDataSize;
    in.read(&audioDataSize, sizeof(uint32_t));
    m_audioDataSize = audioDataSize;
    m_fileName = fileName;
    return audioDataSize;
}

void StreamerClient::pause()
{
    m_playing = false;
    m_player->pause();
}

void StreamerClient::resume()
{
    m_player->resume();
    m_playing = true;
}
