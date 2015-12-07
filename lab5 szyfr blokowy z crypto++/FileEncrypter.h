#pragma once
#include "FileOperator.h"

class FileEncrypter
{
public:
	FileEncrypter();
	void Encrypt();
	void Decrypt();
	void PrintStructure();
	~FileEncrypter();
	void PrintMenu();
private:
	FileOperator fileOperator;
	
};

