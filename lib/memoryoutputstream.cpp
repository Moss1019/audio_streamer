#include "memoryoutputstream.hpp"

#include <cmath>
#include <cstring>

void MemoryOutputStream::reallocate(int newLength)
{
    char *buffer = new char[newLength];
    if(m_buffer != nullptr)
    {
        std::memcpy(buffer, m_buffer, m_head);
        delete[] m_buffer;
    }
    m_buffer = buffer;
    m_capacity = newLength;
}

MemoryOutputStream::MemoryOutputStream()
    :m_buffer{nullptr}, m_head{0}, m_capacity{0}
{
    reallocate(32);
}

MemoryOutputStream::~MemoryOutputStream()
{
    if(m_buffer != nullptr)
    {
        delete[] m_buffer;
        m_buffer = nullptr;
    }
}

char *MemoryOutputStream::getBufferPtr() const
{
    return m_buffer;
}

int MemoryOutputStream::getBufferLength() const
{
    return m_head;
}

void MemoryOutputStream::write(const void *data, unsigned size)
{
    unsigned resultHead = m_head + size;
    if(resultHead > m_capacity) 
    {
        reallocate(std::max(m_capacity * 2, resultHead));
    }
    std::memcpy(m_buffer + m_head, data, size);
    m_head += size;
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
