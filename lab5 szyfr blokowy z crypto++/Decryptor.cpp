#include "Decryptor.h"
#include "Encryptor.h"
#include "ArchiveLoader.h"

Decryptor::Decryptor(FileOperator _fileOperator)
{
	ArchiveLoader archiveLoader(_fileOperator.directory);
}


Decryptor::~Decryptor()
{
}

void Decryptor::LoadFileContent()
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
		crypto.iv[i] = file2.get();
		//IV.push_back(file2.get());
	file2.close();

	std::cout << "IV odtworzone:" << crypto.iv << std::endl;

}

void Decryptor::LoadEncryptedFile()//Joins all blocks to one
{
	std::string CipherText;

	//TODO
}

