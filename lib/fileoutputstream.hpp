#pragma once

#include <string>
#include <fstream>

class FileOutputStream
{
private:
    std::string m_fileName;

    std::ofstream *m_ofStream;

    unsigned m_size;

    unsigned m_offset;

public:
    FileOutputStream(const std::string &fileName);

    ~FileOutputStream();

    void writesome(char *buffer, unsigned length);

    void write(char *buffer, unsigned length) const;
};
