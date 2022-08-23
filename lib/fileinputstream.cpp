#include "fileinputstream.hpp"

#include <fstream>

FileInputStream::FileInputStream(const std::string &fileName)
	:m_fileName{fileName}
{
}

void FileInputStream::read(char **buffer, unsigned &length) const
{
	std::ifstream in(m_fileName, std::ios::binary);
	in.seekg(0, std::ios::end);
	length = in.tellg();
	in.seekg(0, std::ios::beg);
	*buffer = new char[length];
	in.read(*buffer, length);
	in.close();
}
