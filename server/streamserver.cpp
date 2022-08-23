#include "streamserver.hpp"

#include <cstring>

#include <iostream>

#include "wavfile.hpp"
#include "songinfo.hpp"
#include "tcpclient.hpp"
#include "tcpmessage.hpp"
#include "songrequest.hpp"
#include "directoryinfo.hpp"
#include "fileinputstream.hpp"

StreamServer::StreamServer(const SocketAddress &address, onAcceptedCallback onAccepted)
    :m_audioFolder{"../songs"}
{
    m_onReceive = [](TcpClient *client, MemoryInputStream &stream)
    {
        TcpMessage message;
        message.read(stream);
        if(message.messageType == 2001)
        {
            std::vector<std::string> songs;
            DirectoryInfo info("../songs");
            info.getFiles(songs);
            MemoryOutputStream oStream;
            oStream.write((unsigned)songs.size());
            for(const auto &f : songs)
            {
                unsigned size = (unsigned)f.length();
                oStream.write(size);
                oStream.write(f.c_str(), size);
            }
            message.dataLength = oStream.getLength();
            message.data = new char[message.dataLength];
            std::memcpy(message.data, oStream.getBufferPtr(), message.dataLength);
            oStream.empty();
            message.write(oStream);
            client->sendData(oStream);
        }
        else if (message.messageType == 2002)
        {
            char *buffer = new char[message.dataLength];
            std::memcpy(buffer, message.data, message.dataLength);
            std::string fileName(buffer);
            FileInputStream fiStream(fileName);
            delete[] buffer;
            unsigned size;
            fiStream.read(&buffer, size);
            WavFile wavDetails;
            wavDetails.read(buffer);
            SongInfo songDetails;
            songDetails.channels = wavDetails.channels;
            songDetails.length = wavDetails.length;
            songDetails.sampleRate = wavDetails.sampleRate;
            songDetails.size = wavDetails.size;
            MemoryOutputStream oStream;
            songDetails.write(oStream);
            message.dataLength = oStream.getLength();
            delete[] message.data;
            message.data = new char[message.dataLength];
            std::memcpy(message.data, oStream.getBufferPtr(), message.dataLength);
            oStream.empty();
            message.write(oStream);
            client->sendData(oStream);
            delete[] buffer;
        }
        else if (message.messageType == 2003)
        {
            SongRequest request;
            MemoryInputStream iStream(message.data, message.dataLength);
            request.read(iStream);
            FileInputStream ifStream(request.fileName);
            char *buffer;
            unsigned size;
            ifStream.read(&buffer, size);
            WavFile file;
            file.read(buffer);
            MemoryOutputStream oStream;
            unsigned computedLength = request.length;
            if(request.offset + computedLength > file.size)
            {
                computedLength = file.size - request.offset;
            }
            oStream.write(file.frames + request.offset, computedLength);
            message.dataLength = computedLength;
            delete[] message.data;
            message.data = new char[message.dataLength];
            std::memcpy(message.data, oStream.getBufferPtr(), message.dataLength);
            oStream.empty();
            message.write(oStream);
            client->sendData(oStream);
            delete[] buffer;
        }
    };
    m_server = new TcpServer(address, m_onReceive, onAccepted);
}

StreamServer::~StreamServer()
{
    if(m_server != nullptr)
    {
        delete m_server;
        m_server = nullptr;
    }
}

void StreamServer::start()
{
    m_server->start();
}

void StreamServer::stop()
{
    m_server->stop();
}
