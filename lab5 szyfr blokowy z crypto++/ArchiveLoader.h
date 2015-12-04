#pragma once
#include "FileOperator.h"
#include <sstream>
class ArchiveLoader
{
public:
	ArchiveLoader();
	ArchiveLoader(std::string dir);
	~ArchiveLoader();
	std::string FileContent(std::string file_name);
	std::string GetFilesContent(int number_of_files);
	std::string archiveDir;

private:
	FileOperator fileOperator;
	void GetArchiveDir(std::string dir);
};

