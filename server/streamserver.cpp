#include "streamserver.hpp"

#include <cstring>

#include <iostream>

#include "wavfile.hpp"
#include "tcpclient.hpp"
#include "tcpmessage.hpp"
#include "directoryinfo.hpp"
#include "fileinputstream.hpp"
#include "memoryoutputstream.hpp"

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
            message.dataLength = oStream.getBufferLength();
            message.data = new char[message.dataLength];
            std::memcpy(message.data, oStream.getBufferPtr(), message.dataLength);
            oStream.empty();
            message.write(oStream);
            client->sendData(oStream);
        }
        else if (message.messageType == 2002)
        {
            MemoryInputStream fileNameStream(message.data, message.dataLength);
            char *fileNameBuffer = new char[message.dataLength];
            fileNameStream.read(fileNameBuffer, message.dataLength);
            std::string fileName(fileNameBuffer, message.dataLength);
            FileInputStream audioFile(fileName);
            char *audioFileBuffer = new char[audioFile.size()];
            audioFile.read(audioFileBuffer);
            WavFile audioFileInfo(audioFileBuffer);
            MemoryOutputStream fileInfoOut;
            fileInfoOut.write(audioFileInfo.audioDataSize);
            delete[] audioFileBuffer;
            delete[] fileNameBuffer;
            message.dataLength = fileInfoOut.getBufferLength();
            message.data = new char[message.dataLength];
            std::memcpy(message.data, fileInfoOut.getBufferPtr(), message.dataLength);
            fileInfoOut.empty();
            message.write(fileInfoOut);
            client->sendData(fileInfoOut);
        }
        else if (message.messageType == 2003)
        {
            MemoryInputStream in(message.data, message.dataLength);
            unsigned offset;
            in.read(offset);
            unsigned length;
            in.read(length);
            unsigned fileNameLength;
            in.read(fileNameLength);
            char *buffer = new char[fileNameLength];
            in.read(buffer, fileNameLength);
            std::string fileName(buffer, fileNameLength);
            delete[] buffer;
            FileInputStream file(fileName);
            buffer = new char[file.size()];
            file.read(buffer);
            WavFile wavFile(buffer);
            delete[] buffer;
            unsigned computedLength = length;
            if(offset + computedLength > wavFile.audioDataSize)
            {
                computedLength = wavFile.audioDataSize - offset;
            }
            message.dataLength = length;
            delete[] message.data;
            message.data = new char[length];
            std::memcpy(message.data, wavFile.audioData + offset, length);
            MemoryOutputStream out;
            message.write(out);
            client->sendData(out);
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
