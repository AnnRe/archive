#include "Decryptor.h"
#include "Encryptor.h"
#include "ArchiveLoader.h"

Decryptor::Decryptor(FileOperator _fileOperator)
{
	ArchiveLoader _archiveLoader(_fileOperator.directory);
	archiveLoader = _archiveLoader;
	dataFileOperator = _fileOperator;
}


Decryptor::~Decryptor()
{
}

void Decryptor::LoadFileContent()
{

}

void Decryptor::LoadConfiguration()
{
	std::string dir=archiveLoader.archiveDir;
	std::string filesContent;
	
	int numberOfFiles = (CryptoPP::AES::BLOCKSIZE + numberOfDigits) / dataFileOperator.BundleSize();
	std::cout << "!!" << std::endl;
	std::cout << CryptoPP::AES::BLOCKSIZE << "; " << numberOfDigits << "; " << dataFileOperator.BundleSize() << std::endl;
	filesContent = archiveLoader.GetFilesContent(numberOfFiles);

	std::string len=filesContent.substr(numberOfDigits);
	std::istringstream iss(len);
	int dl; iss >> dl;

	//reading initialization string IV
	std::string IV;
	for (int i = numberOfDigits; i < numberOfDigits+CryptoPP::AES::BLOCKSIZE; i++)
		crypto.iv[i] = filesContent[i];
	//IV.push_back(file2.get());

	std::cout << "IV odtworzone:" << crypto.iv << std::endl;
}

void Decryptor::LoadEncryptedFile()//Joins all blocks to one
{
	std::string CipherText;

	//TODO
}

