#include "memoryoutputstream.hpp"

#include <memory>
#include <cstring>

void MemoryOutputStream::reallocate(int newLength)
{
	m_buffer = static_cast<char *>(std::realloc(m_buffer, newLength));
	m_capacity = newLength;
}

MemoryOutputStream::MemoryOutputStream()
	:m_buffer(nullptr), m_head(0), m_capacity(0)
{
	reallocate(32);
}

MemoryOutputStream::~MemoryOutputStream()
{
	if (m_buffer != nullptr)
	{
		std::free(m_buffer);
		m_buffer = nullptr;
	}
}

char *MemoryOutputStream::getBufferPtr() const
{
	return m_buffer;
}

int MemoryOutputStream::getLength() const
{
	return m_head;
}

void MemoryOutputStream::write(const void *data, unsigned byteCount)
{
	unsigned resultHead = m_head + byteCount;
	if (resultHead > m_capacity)
	{
		reallocate(std::max(m_capacity * 2, resultHead));
	}
	std::memcpy(m_buffer + m_head, data, byteCount);
	m_head = resultHead;
}

void MemoryOutputStream::write(int data)
{
	write(&data, sizeof(int));
}

void MemoryOutputStream::write(unsigned data)
{
	write(&data, sizeof(unsigned));
}

void MemoryOutputStream::empty()
{
	m_head = 0;
}
