#include "ArchiveLoader.h"
#include <string>
#include "FileOperator.h"

ArchiveLoader::ArchiveLoader()
{}

ArchiveLoader::ArchiveLoader(std::string dir)
{
	GetArchiveDir(dir);
	FileOperator fop(archiveDir);
	fileOperator = fop;

	fileOperator.GetFileStructure();
	fileOperator.ListFileStructure();

}


ArchiveLoader::~ArchiveLoader()
{
}

void ArchiveLoader::GetArchiveDir(std::string dir)
{
	size_t poz = dir.find_last_of("\\");
	archiveDir = dir.substr(0,poz+1);
	archiveDir += "archive";
	std::cout << "Archive dir:" << archiveDir << std::endl;
}

std::string GetContent()
{

}
