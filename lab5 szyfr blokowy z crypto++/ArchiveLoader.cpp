#include "ArchiveLoader.h"
#include <string>
#include "FileOperator.h"

ArchiveLoader::ArchiveLoader()
{}

ArchiveLoader::ArchiveLoader(std::string dir)
{
	GetArchiveDir(dir);
	FileOperator fop(archiveDir);
	archiveFileOperator = fop;
}


std::string ArchiveLoader::GetFilesContent(int number_of_files)
{
	//std::cout << "loading " << number_of_files << " files...\n";
	std::string content = "";

	for (int i = 1; i < number_of_files+1; i++)
	{
		std::ostringstream oss; oss << i;

		std::string fileName = archiveDir + "\\bundle"+oss.str()+".txt";
		content += FileContent(fileName);
	}
	return content;
}

ArchiveLoader::~ArchiveLoader()
{
}

std::string ArchiveLoader::FileContent(std::string file_name)
{
	std::ifstream file(file_name);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();

}

void ArchiveLoader::GetArchiveDir(std::string dir)
{
	size_t poz = dir.find_last_of("\\");
	archiveDir = dir.substr(0,poz+1);
	archiveDir += "archive";
	
}

int ArchiveLoader::GetNumberOfFiles()
{
	return archiveFileOperator.NumberOfFiles();
}