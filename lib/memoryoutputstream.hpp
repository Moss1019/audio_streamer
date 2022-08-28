#pragma once

class MemoryOutputStream
{
private:
    char *m_buffer;

    unsigned m_head;

    unsigned m_capacity;

    void reallocate(int newLength);

public:
    MemoryOutputStream();

    MemoryOutputStream(const MemoryOutputStream &other) = delete;

    MemoryOutputStream operator=(const MemoryOutputStream &other) = delete;

    ~MemoryOutputStream();

    char *getBufferPtr() const;

    int getBufferLength() const;

    void write(const void *data, unsigned size);

    void write(int data);

    void write(unsigned data);

    void empty();
};
