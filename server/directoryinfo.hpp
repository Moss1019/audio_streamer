#pragma once

#include <vector>
#include <string>

class DirectoryInfo
{
private:
	std::string m_folder;

public:
	DirectoryInfo(const std::string &folder);

	DirectoryInfo(const DirectoryInfo &other) = delete;

	DirectoryInfo operator=(const DirectoryInfo &other) = delete;

	void getFiles(std::vector<std::string> &files) const;
};
