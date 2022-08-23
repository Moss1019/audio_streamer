#pragma once

#include <string>

class FileOutputStream
{
private:
	std::string m_fileName;

public:
	FileOutputStream(const std::string &fileName);

	FileOutputStream(const FileOutputStream &other) = delete;

	FileOutputStream operator=(const FileOutputStream &other) = delete;

	void write(char *buffer, unsigned length) const;
};

