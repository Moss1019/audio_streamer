#pragma once

#include <memory>

#include <iostream>

#include "memoryinputstream.hpp"
#include "memoryoutputstream.hpp"

struct TcpMessage
{
	int messageType;

	unsigned dataLength;

	char *data;

	TcpMessage()
		:messageType{0}, dataLength{0}, data{nullptr}
	{}

	~TcpMessage()
	{
		if (data != nullptr)
		{
			delete[] data;
			data = nullptr;
		}
	}

	void read(MemoryInputStream &stream)
	{
		stream.read(&messageType, sizeof(int));
		stream.read(&dataLength, sizeof(unsigned));
		if(dataLength > 0)
		{
			data = new char[dataLength];
			stream.read(data, dataLength);
		}
	}

	void write(MemoryOutputStream &stream) const
	{
		stream.write(&messageType, sizeof(int));
		stream.write(&dataLength, sizeof(unsigned));
		stream.write(data, dataLength);
	}
};
