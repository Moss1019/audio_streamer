#include "fileoutputstream.hpp"

FileOutputStream::FileOutputStream(const std::string &fileName)
    :m_fileName{fileName}, m_size{0}, m_offset{0}
{
    m_ofStream = new std::ofstream(m_fileName.c_str(), std::ios::binary);
}

FileOutputStream::~FileOutputStream()
{
    if(m_ofStream != nullptr)
    {
        m_ofStream->close();
        delete m_ofStream;
        m_ofStream = nullptr;
    }
}

void FileOutputStream::writesome(char *buffer, unsigned length)
{
    m_ofStream->seekp(m_offset, std::ios::beg);
    m_ofStream->write(buffer, length);
    m_offset += length;
}

void FileOutputStream::write(char *buffer, unsigned length) const
{
    m_ofStream->seekp(0, std::ios::beg);
    m_ofStream->write(buffer, length);
}