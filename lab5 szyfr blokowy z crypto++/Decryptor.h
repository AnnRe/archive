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
	void LoadConfiguration();
	void LoadEncryptedFile();
	void LoadFileContent();

private:
	byte iv;
	int fileSize;
	int numberOfDigits;
	
	ArchiveLoader archiveLoader;

	std::string archive_path;
	Crypto crypto;

};

