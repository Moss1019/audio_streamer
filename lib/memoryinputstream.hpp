#pragma once
class MemoryInputStream
{
private:
	char *m_buffer;

	unsigned m_head;

	unsigned m_capacity;

public:
	MemoryInputStream(char *buffer, int bufferSize);

	MemoryInputStream(const char *buffer, int bufferSize);

	~MemoryInputStream();

	unsigned getRemainingSize() const;

	void read(void *data, unsigned byteCount);

	void read(int &data);

	void read(unsigned &data);

	void setBuffer(void *buffer, int bufferSize);
};

