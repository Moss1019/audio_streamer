#include "fileinputstream.hpp"

FileInputStream::FileInputStream(const std::string &fileName)
    :m_fileName{fileName}, m_size{0}, m_offset{0}
{
    m_ifStream = new std::ifstream(m_fileName.c_str(), std::ios::binary);
    m_ifStream->seekg(0, std::ios::end);
    m_size = m_ifStream->tellg();
    m_ifStream->seekg(0, std::ios::beg);
}

FileInputStream::~FileInputStream()
{
    if(m_ifStream != nullptr)
    {
        m_ifStream->close();
        delete m_ifStream;
        m_ifStream = nullptr;
    }
}

 unsigned FileInputStream::size() const
{
    return m_size;
}

unsigned FileInputStream::offset() const
{
    return m_offset;
}

unsigned FileInputStream::readsome(char *buffer, unsigned length)
{
    unsigned newOffset = m_offset + length;
    if(newOffset > m_size)
    {
        length = newOffset - m_size;
    }
    m_ifStream->seekg(m_offset, std::ios::beg);
    m_ifStream->read(buffer, length);
    m_offset += length;
    return length;
}

void FileInputStream::read(char *buffer)
{
    m_ifStream->seekg(0, std::ios::beg);
    m_ifStream->read(buffer, m_size);
    m_ifStream->seekg(m_offset, std::ios::beg);
}
