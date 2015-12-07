#pragma once
#include<string>
#include <vector>
#include<map>
#include<Windows.h>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include<iostream>
#include<sys/stat.h>
#include<fstream>
class FileOperator
{
public:
	void LoadSizeStringSize();
	void LoadConfigurationAndStructure();
	void GetTotalLength();
	void GetConfiguration();
	FileOperator(std::string dir);
	FileOperator();
	~FileOperator();
	//std::string GetArchiveDir();
	std::vector<std::string> fileNames;
	std::map<std::string, int> fileSizes;//w przypadku katalogow -1 tzn zapisana jego struktura -2 nie
	std::map<std::string, std::string> fileTypes;
	std::string GetTotalContent();
	int totalLength;//sum of all files length

	int NumberOfFiles();
	int BundleSize();
	std::string GetArchiveDir() const;
	void CreateArchiveDir();
	void GetFileStructure();
	void ListFileStructure();
	std::string directory;
	int numberOfDigits;


private:
	int bundleSize;

	bool FirstLogon();
	void GetPath();
	void GetFileStructure(std::string path);
	static int CountChars(std::string path);
	void SaveFileStructure(std::string fileName);

	int calculateDepth(std::string filePath);//liczy jak bardzo zagnie¿d¿ony jest katalog
	void GetBundleSize();

	void LoadPath();
	void LoadBundleSize();
	void LoadFileStructure(std::string fileName);



};


