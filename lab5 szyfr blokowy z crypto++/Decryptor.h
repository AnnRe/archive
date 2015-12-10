#pragma once
#include <cryptlib.h>
#include "Crypto.h"
#include "ArchiveLoader.h"
#include "FileOperator.h"

class Decryptor
{
public:
	Decryptor(FileOperator _fileOperator);
	~Decryptor();
	void Decrypt(std::string key);
	void SaveToFiles();
	void Run(std::string key);
	
private:
	int fileSize;
	
	ArchiveLoader archiveLoader;
	FileOperator dataFileOperator;
	std::string archive_path;
	Crypto crypto;
	std::string cipherText;

	void LoadConfiguration();
	void LoadEncryptedFile();


};

