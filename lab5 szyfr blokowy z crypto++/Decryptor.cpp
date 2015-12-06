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
	std::cout << "len:" << cipherText.length()<<std::endl;
	//int blockSize = AES::BLOCKSIZE;
	for (int i = 0; i < cipherText.length(); i += AES::BLOCKSIZE)
	{
		crypto.GetNextAESkey();
		crypto.GetNextH();
		//Decryptor
		std::string key;
		StringSource ss("28292A2B2D2E2F30323334353738393A3C3D3E3F41424344464748494B4C4D4E", true,
			new HexDecoder(
			new StringSink(key)));
		
		/****previousRecoveredText = EncryptedBlockText;
		byte previousG[16]; for (int k = 0; k < 16; k++) previousG[k] = crypto.G[k];

		crypto.GetNextG(previousRecoveredText);

			/****for (int j = 0; j < 16; j++)
				RecoveredText[j] ^= previousG[j];***/
		
		byte encryptedTextBlock[AES::BLOCKSIZE]; byte decryptedTextBlock[AES::BLOCKSIZE];
		
		for (int l = i; l < i+AES::BLOCKSIZE; l++)
			encryptedTextBlock[l%AES::BLOCKSIZE] = cipherText[l];
		
		std::cout << "^H:" /*<< crypto.H */<< std::endl;

		for (int j = 0; j < AES::BLOCKSIZE; j++) //H xor textblock
		{
			encryptedTextBlock[j] -= crypto.H[j];
			previousEncryptedTextBlock[j] = encryptedTextBlock[j];
		}
		// Decryption AES
		AES::Decryption alg2;
		alg2.SetKey((byte*)key.c_str(), 32);
		alg2.ProcessBlock(encryptedTextBlock, decryptedTextBlock);
			
			/*std::cout << "\tkey:" << crypto.key << std::endl;
			std::cout << "recovered:" << RecoveredText << std::endl;*****/

		/*****}
		catch (Exception &e)		{			std::cout << e.what() << std::endl;		}****/
		/*****previousRecoveredText = RecoveredText;*/
		crypto.GetNextH();

		/****for (int j = 0; j < EncryptedBlockText.length(); j++)
		{
			unsigned char c = EncryptedBlockText[j] ^ crypto.H[j];
			EncryptedBlockText[j] = c;
		}****/

		/**** crypto.GetNextG(previousRecoveredText); ****/
		for (int k = 0; k < AES::BLOCKSIZE; k++)
			decryptedText.push_back(decryptedTextBlock[k]);
	}
	
	std::cout << "cipher:"<<cipherText << std::endl;
	std::cout << "text:"<<decryptedText << std::endl;
	/*****std::cout << cipherText << std::endl;****/
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
	std::cout << "blocksize:"<<AES::BLOCKSIZE << "; " << "digits:"<<digits << "; " << "bundlesize:"<<dataFileOperator.BundleSize() << std::endl;
	filesContent = archiveLoader.GetFilesContent(numberOfFiles);

	//std::cout << "loaded ciphertext:" << filesContent << std::endl;
	std::string len = filesContent.substr(0,digits);
	std::istringstream iss(len);
	int dl; iss >> dl;

	//reading initialization string IV

	for (int i = digits; i < digits + CryptoPP::AES::BLOCKSIZE; i++)
	{
		crypto.iv[i - digits] = filesContent[i];
	}
	//IV.push_back(file2.get());

	std::cout << "IV odtworzone:";
	for(int k=0;k<AES::BLOCKSIZE;k++)
		std::cout<< crypto.iv[k] ;
	std::cout << std::endl;
	/*std::cout<<"pojedynczo:\n";
	for (int i = 0; i < sizeof crypto.iv; i++)
	{
		std::cout << crypto.iv[i]<< std::endl;
	}
	std::cout << "Key:" << crypto.key << std::endl;*/
}

void Decryptor::LoadEncryptedFile()//Joins all blocks to one
{
	std::string CipherText;
	int n=archiveLoader.GetNumberOfFiles();
	CipherText = archiveLoader.GetFilesContent(n);
	size_t poz = dataFileOperator.numberOfDigits + CryptoPP::AES::BLOCKSIZE;
	cipherText = CipherText.substr(poz);

}

