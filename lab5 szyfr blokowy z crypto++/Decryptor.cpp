#include "Decryptor.h"
#include "Encryptor.h"

Decryptor::Decryptor(FileOperator _fileOperator)
{
	fileOperator = _fileOperator;
	numberOfFiles = GetNumberOfFiles();
}


Decryptor::~Decryptor()
{
}

void Decryptor::LoadConfiguration()
{
	std::ifstream file2("output.txt");
	std::string len;
	for (int i = 0; i < numberOfDigits;i++)
		len += file2.get();
	std::istringstream iss(len);
	int dl; iss >> dl;

	//reading initialization string IV
	std::string IV;
	for (int i = 0; i < CryptoPP::AES::BLOCKSIZE; i++)
		IV.push_back(file2.get());
	iv = (unsigned)IV.c_str();
	file2.close();
}

void Decryptor::LoadEncryptedFile()//Joins all blocks to one
{
	std::string CipherText;

	int numberOfFiles = GetNumberOfFiles();
	for (int i = 0; i < numberOfFiles; i++)
	{
		///TODO
	}
}

int Decryptor::GetNumberOfFiles()
{
	return fileOperator.NumberOfFiles();
}
