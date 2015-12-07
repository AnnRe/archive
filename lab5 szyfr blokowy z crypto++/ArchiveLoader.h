#pragma once
#include "FileOperator.h"
#include <sstream>
class ArchiveLoader
{
public:
	ArchiveLoader();
	ArchiveLoader(std::string dir);
	~ArchiveLoader();
	static std::string FileContent(std::string file_name);
	std::string GetFilesContent(int number_of_files);
	std::string archiveDir;
	int GetNumberOfFiles();


private:
	FileOperator archiveFileOperator;
	void GetArchiveDir(std::string dir);
};

