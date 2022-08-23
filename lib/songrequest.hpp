#pragma once

#include "memoryinputstream.hpp"
#include "memoryoutputstream.hpp"

struct SongRequest
{
    unsigned offset;

    unsigned length;

    unsigned fileNameSize;

    std::string fileName;

    void write(MemoryOutputStream &stream)
    {
        stream.write(offset);
        stream.write(length);
        fileNameSize = fileName.length();
        stream.write(fileNameSize);
        stream.write(fileName.c_str(), fileNameSize);
    }

    void read(MemoryInputStream &stream)
    {
        stream.read(offset);
        stream.read(length);
        stream.read(fileNameSize);
        char fileNameBuffer[fileNameSize];
        stream.read(fileNameBuffer, fileNameSize);
        fileName = std::string(fileNameBuffer);
    }
};
