#pragma once

class MemoryInputStream
{
private:
    char *m_buffer;

    unsigned m_head;

    unsigned m_capacity;
public:
    MemoryInputStream(char *buffer, unsigned size);

    MemoryInputStream(const MemoryInputStream &other) = delete;

    MemoryInputStream operator=(const MemoryInputStream &other) = delete;

    ~MemoryInputStream();

    unsigned getReminingSize() const;

    void read(void *data, unsigned size);

    void read(int &data);

    void read(unsigned &data);

    void setBuffer(char *buffer, unsigned size);
};
