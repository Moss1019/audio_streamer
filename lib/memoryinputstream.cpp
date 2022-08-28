#include "memoryinputstream.hpp"

#include <cstring>

MemoryInputStream::MemoryInputStream(char *buffer, unsigned size)
    :m_buffer{nullptr}, m_head{0}, m_capacity{size}
{
    m_buffer = new char[size];
    std::memcpy(m_buffer, buffer, size);
}

MemoryInputStream::~MemoryInputStream()
{
    if(m_buffer != nullptr)
    {
        delete m_buffer;
        m_buffer = nullptr;
    }
}

unsigned MemoryInputStream::getReminingSize() const
{
    return m_capacity - m_head;
}

void MemoryInputStream::read(void *data, unsigned size)
{
    std::memcpy(data, m_buffer + m_head, size);
    m_head += size;
}

void MemoryInputStream::read(int &data)
{
    read(&data, sizeof(int));
}

void MemoryInputStream::read(unsigned &data)
{
    read(&data, sizeof(unsigned));
}

void MemoryInputStream::setBuffer(char *buffer, unsigned size)
{
    m_head = 0;
    if(m_buffer != nullptr)
    {
        delete[] m_buffer;
    }
    m_buffer = new char[size];
    std::memcpy(m_buffer, buffer, size);
    m_capacity = size;
}
