#include "directoryinfo.hpp"

#include <filesystem>

namespace fs = std::filesystem;

DirectoryInfo::DirectoryInfo(const std::string &folder)
	:m_folder{folder}
{
}

void DirectoryInfo::getFiles(std::vector<std::string> &files) const
{
	files.clear();
	for (const auto &f : fs::directory_iterator(m_folder))
	{
		files.push_back(std::string(f.path().c_str()));
	}
}
