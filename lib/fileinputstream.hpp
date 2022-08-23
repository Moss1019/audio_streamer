#pragma once

#include <string>

class FileInputStream
{
private:
	std::string m_fileName;

public:
	FileInputStream(const std::string &fileName);

	FileInputStream(const FileInputStream &other) = delete;

	FileInputStream operator=(const FileInputStream &other) = delete;

	void read(char **buffer, unsigned &length) const;
};
