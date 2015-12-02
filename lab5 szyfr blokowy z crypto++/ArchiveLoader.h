#pragma once
#include "FileOperator.h"

class ArchiveLoader
{
public:
	ArchiveLoader();
	ArchiveLoader(std::string dir);
	~ArchiveLoader();
private:
	FileOperator fileOperator;
	std::string archiveDir;
	void GetArchiveDir(std::string dir);
};

