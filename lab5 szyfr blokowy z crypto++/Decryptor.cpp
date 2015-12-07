#include "Decryptor.h"
#include "ArchiveLoader.h"
//#include <modes.h>
#include <hex.h>
#include<sstream>

using namespace CryptoPP;
Decryptor::Decryptor(FileOperator _fileOperator)
{
	ArchiveLoader _archiveLoader(_fileOperator.directory);
	archiveLoader = _archiveLoader;
	dataFileOperator = _fileOperator;
}

Decryptor::~Decryptor()
{
}

void Decryptor::Decrypt()//TODO:key!
{
	byte previousEncryptedTextBlock[AES::BLOCKSIZE];
	crypto.GetFirstG();
	std::string decryptedText = "";
	std::cout << "----DESZYFROWANIE...\n";
	for (int i = 0; i < cipherText.length(); i += AES::BLOCKSIZE)
	{
		crypto.GetNextAESkey();
		crypto.GetNextH();
		//Decryptor
		std::string key;
		StringSource ss("28292A2B2D2E2F30323334353738393A3C3D3E3F41424344464748494B4C4D4E", true,
			new HexDecoder(
			new StringSink(key)));
		
		byte encryptedTextBlock[AES::BLOCKSIZE]; byte decryptedTextBlock[AES::BLOCKSIZE];
		
		for (int l = i; l < i+AES::BLOCKSIZE; l++)
			encryptedTextBlock[l%AES::BLOCKSIZE] = cipherText[l];
		
		for (int j = 0; j < AES::BLOCKSIZE; j++) //H xor textblock
		{
			encryptedTextBlock[j] -= crypto.H[j];
			previousEncryptedTextBlock[j] = encryptedTextBlock[j];
		}
		//g1
		crypto.GetNextG(previousEncryptedTextBlock);
		// Decryption AES
		AES::Decryption alg2;
		alg2.SetKey((byte*)key.c_str(), 32);
		alg2.ProcessBlock(encryptedTextBlock, decryptedTextBlock);

		//block ^G
		for (int j = 0; j < AES::BLOCKSIZE; j++)
		{
			decryptedTextBlock[j] ^=crypto.Gprevious[j];
		}
		crypto.GetNextH();

		for (int k = 0; k < AES::BLOCKSIZE; k++)
			decryptedText.push_back(decryptedTextBlock[k]);
	}
}

void Decryptor::Run()
{
	LoadConfiguration();
	LoadEncryptedFile();
	Decrypt();
}

void Decryptor::LoadConfiguration()
{
	crypto.Initialize();
	std::string dir=archiveLoader.archiveDir;
	std::string filesContent;
	int digits = dataFileOperator.numberOfDigits;
	
	int numberOfFiles = (AES::BLOCKSIZE + digits) / dataFileOperator.BundleSize()+1;
	filesContent = archiveLoader.GetFilesContent(numberOfFiles);

	std::string len = filesContent.substr(0,digits);
	std::istringstream iss(len);
	int dl; iss >> dl;

	//reading initialization string IV
	for (int i = digits; i < digits + CryptoPP::AES::BLOCKSIZE; i++)
	{
		crypto.iv[i - digits] = filesContent[i];
	}
}

void Decryptor::LoadEncryptedFile()//Joins all blocks to one
{
	std::string CipherText;
	int n=archiveLoader.GetNumberOfFiles();
	CipherText = archiveLoader.GetFilesContent(n);
	size_t poz = dataFileOperator.numberOfDigits + CryptoPP::AES::BLOCKSIZE;
	cipherText = CipherText.substr(poz+1);

}

