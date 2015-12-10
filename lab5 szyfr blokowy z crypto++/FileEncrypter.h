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
	void Refresh();
	void ChangePath();
	void GetPass(std::string pasw);
	static void PrintMenu();
private:
	FileOperator fileOperator;
	std::string pass;
	
};

