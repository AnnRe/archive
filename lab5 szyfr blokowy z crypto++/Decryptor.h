#pragma once
#include <cryptlib.h>
#include "FileOperator.h"

class Decryptor
{
public:
	Decryptor(FileOperator _fileOperator);
	~Decryptor();
	void LoadConfiguration();
	void LoadEncryptedFile();
private:
	byte iv;
	int fileSize;
	int numberOfDigits;
	int numberOfFiles;
	int GetNumberOfFiles();
	FileOperator fileOperator;

};

