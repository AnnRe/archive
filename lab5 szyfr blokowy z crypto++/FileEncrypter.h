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
	static void PrintMenu();
private:
	FileOperator fileOperator;
	
};

