#pragma once

#include <string>
#include <fstream>

class FileInputStream
{
private:
    std::string m_fileName;

    std::ifstream *m_ifStream;

    unsigned m_size;

    unsigned m_offset;

public:
    FileInputStream(const std::string &fileName);

    ~FileInputStream();

    unsigned size() const;

    unsigned offset() const;

    unsigned readsome(char *buffer, unsigned length);

    void read(char *buffer);
};