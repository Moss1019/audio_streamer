#include "streamerclient.hpp"

#include <chrono>

#include <iostream>

#include "tcpmessage.hpp"
#include "songrequest.hpp"

void StreamerClient::doWork()
{
    unsigned bytesChunkSize = 5 * 2 * 2 * 44100;
    while(m_isRunning)
    {
        if(m_isPlaying)
        {
            m_locked = true;
            if(m_song != nullptr && m_bytesPlayed < m_song->size && m_player->numberDataChunks() < 2)
            {
                SongRequest request;
                request.fileName = m_fileName;
                request.fileNameSize = m_fileName.length();
                request.length = bytesChunkSize;
                request.offset = m_bytesPlayed;
                std::cout << "Getting " << m_bytesPlayed << "\n";
                MemoryOutputStream oStream;
                request.write(oStream);
                TcpMessage message;
                message.messageType = 2003;
                message.dataLength = oStream.getLength();
                message.data = new char[oStream.getLength()];
                std::memcpy(message.data, oStream.getBufferPtr(), oStream.getLength());
                oStream.empty();
                message.write(oStream);
                int sent = m_socket->sendData(oStream);
                oStream.empty();
                int received = m_socket->receiveData(oStream);
                MemoryInputStream iStream(oStream.getBufferPtr(), oStream.getLength());
                message.read(iStream);
                m_player->play(message.data, message.dataLength);
                m_bytesPlayed += message.dataLength;
            }
            m_locked = false;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(17));
            m_locked = false;
        }
    }
    m_player->stop();
}

StreamerClient::StreamerClient(SocketAddress &serverAddr)
    :m_isRunning{false}, m_isPlaying{false}, m_locked{false}, m_socket{nullptr}, m_player{nullptr}, m_song{nullptr}, m_worker{nullptr}, m_bytesPlayed{0}
{
    m_socket = new TcpSocket(serverAddr);
    m_player = new WavPlayer(44100, 2);
    m_worker = new std::thread(&StreamerClient::doWork, this);
}

StreamerClient::~StreamerClient()
{
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
    if(m_worker != nullptr)
    {
        m_worker->join();
        delete m_worker;
        m_worker = nullptr;
    }
}

void StreamerClient::getSongs(std::vector<std::string> &files)
{
    bool shouldPlay = m_isPlaying;
    if(m_isPlaying) 
    {
        pause();
        while(m_locked) ;
    }
    TcpMessage message;
    message.messageType = 2001;
    MemoryOutputStream out;
    message.write(out);
    m_socket->sendData(out);
    out.empty();
    int received = m_socket->receiveData(out);
    MemoryInputStream in(out.getBufferPtr(), out.getLength());
    message.read(in);
    MemoryInputStream resIn(message.data, message.dataLength);
    unsigned count;
    resIn.read(count);
    files.clear();
    for(unsigned i = 0; i < count; ++i)
    {
        unsigned size;
        resIn.read(size);
        char *data = new char[size];
        resIn.read(data, size);
        files.push_back(std::string(data));
        delete[] data;
    }
    if(shouldPlay)
    {
        resume();
    }
}

void StreamerClient::play(const std::string &fileName)
{
    pause();
    while(m_locked) ;
    m_fileName = fileName;
    TcpMessage message;
    message.messageType = 2002;
    message.dataLength = fileName.size();
    message.data = new char[message.dataLength];
    std::memcpy(message.data, fileName.c_str(), message.dataLength);
    MemoryOutputStream oStream;
    message.write(oStream);
    m_socket->sendData(oStream);
    oStream.empty();
    int received = m_socket->receiveData(oStream);
    MemoryInputStream iStream(oStream.getBufferPtr(), oStream.getLength());
    message.read(iStream);
    iStream.setBuffer(message.data, message.dataLength);
    if(m_song != nullptr)
    {
        delete m_song;
        m_song = nullptr;
    }
    m_song = new SongInfo();
    m_song->read(iStream);
    m_bytesPlayed = 0;
    resume();
}

void StreamerClient::start()
{
    m_isRunning = true;
    m_isPlaying = false;
}

void StreamerClient::stop()
{
    m_isPlaying = false;
    m_isRunning = false;
}

void StreamerClient::pause()
{
    m_isPlaying = false;
    m_player->pause();
}

void StreamerClient::resume()
{
    m_isPlaying = true;
    m_player->resume();
}

bool StreamerClient::inError() const 
{
    return m_socket->inError();
}

const std::string &StreamerClient::errorMsg() const 
{
    return m_socket->errorMsg();
}
