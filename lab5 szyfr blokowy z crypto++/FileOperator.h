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
	FileOperator(std::string dir,bool archive=false);
	bool StructureFileExists();
	void LoadConfiguration();
	FileOperator();
	~FileOperator();
	void DeleteDir();
	static std::string GetName(std::string fileNameWithExtension);
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
	void GetFileStructure(bool withSavingStructure=false);
	void ListFileStructure();
	std::string directory;
	int numberOfDigits;
	std::string FilesText;
	void LoadFileStructure(std::string fileName);

private:
	int bundleSize;

	bool AppConfigComplete();
	void GetPath();
	void GetFileStructure(std::string path);
	static int CountChars(std::string path);
	void SaveFileStructure(std::string fileName = "structure.txt");

	int CalculateDepth(std::string filePath);//liczy jak bardzo zagnie¿d¿ony jest katalog
	void GetBundleSize();

	void LoadPath();
	void LoadBundleSize();
	bool ConfigurationSet();



};


