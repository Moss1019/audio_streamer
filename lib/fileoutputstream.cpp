#include "fileoutputstream.hpp"

#include <fstream>

FileOutputStream::FileOutputStream(const std::string &fileName)
	:m_fileName{fileName}
{
}

void FileOutputStream::write(char *buffer, unsigned length) const
{
	std::ofstream ofStream(m_fileName, std::ios::binary);
	ofStream.write(buffer, length);
	ofStream.close();
}
