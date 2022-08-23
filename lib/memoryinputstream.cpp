#include "memoryinputstream.hpp"

#include <memory>
#include <cstring>

MemoryInputStream::MemoryInputStream(char *buffer, int bufferSize)
	:m_head(0), m_capacity(bufferSize)
{
	m_buffer = reinterpret_cast<char *>(std::malloc(bufferSize));
	std::memcpy(m_buffer, buffer, bufferSize);
}

MemoryInputStream::MemoryInputStream(const char *buffer, int bufferSize)
	: m_head(0), m_capacity(bufferSize)
{
	m_buffer = reinterpret_cast<char *>(std::malloc(bufferSize));
	std::memcpy(m_buffer, buffer, bufferSize);
}

MemoryInputStream::~MemoryInputStream()
{
	if (m_buffer != nullptr)
	{
		std::free(m_buffer);
		m_buffer = nullptr;
	}
}

unsigned MemoryInputStream::getRemainingSize() const
{
	return m_capacity - m_head;
}

void MemoryInputStream::read(void *data, unsigned byteCount)
{
	std::memcpy(data, m_buffer + m_head, byteCount);
	m_head += byteCount;
}

void MemoryInputStream::read(int &data)
{
	read(&data, sizeof(int));
}

void MemoryInputStream::read(unsigned &data)
{
	read(&data, sizeof(unsigned));
}

void MemoryInputStream::setBuffer(void *buffer, int bufferSize)
{
	m_head = 0;
	m_capacity = bufferSize;
	if(m_buffer != nullptr)
	{
		std::free(m_buffer);
		m_buffer = nullptr;
	}
	m_buffer = reinterpret_cast<char *>(std::malloc(bufferSize));
	std::memcpy(m_buffer, buffer, bufferSize);
}
