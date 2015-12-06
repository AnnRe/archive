#include "Decryptor.h"
#include "ArchiveLoader.h"
#include <modes.h>
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
	crypto.GetFirstG();
	std::cout << "----DESZYFROWANIE...\n";
	std::cout << "len:" << cipherText.length();
	//int blockSize = AES::BLOCKSIZE;
	for (int i = 0; i < cipherText.length(); i += AES::BLOCKSIZE)
	{
		std::string previousRecoveredText;
		std::string PlainText;

		crypto.GetNextAESkey();
		crypto.GetNextH();
		//Decryptor
		std::string key;
		StringSource ss("28292A2B2D2E2F30323334353738393A3C3D3E3F41424344464748494B4C4D4E", true,
			new HexDecoder(
			new StringSink(key)));
		/*****
		CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption
			decryptor(crypto.key, sizeof(crypto.key), crypto.iv);******/
		/*****std::string EncryptedBlockText = "";
		for (int j = i; j < i + 32; j++)
		{
			EncryptedBlockText.push_back(cipherText[j]);
		}
		std::cout << "++++++++\nBlock:" << EncryptedBlockText << "(len=" << EncryptedBlockText.length() << ")" << std::endl;*****/
		/*while (EncryptedBlockText.size() < AES::BLOCKSIZE)
			EncryptedBlockText.push_back('x');*/

		/****for (int j = 0; j < EncryptedBlockText.length(); j++) //H xor textblock
			EncryptedBlockText[j] = (unsigned)EncryptedBlockText[j] ^ crypto.H[j];*****/
		/****previousRecoveredText = EncryptedBlockText;
		byte previousG[16]; for (int k = 0; k < 16; k++) previousG[k] = crypto.G[k];

		crypto.GetNextG(previousRecoveredText);

		// Decryption AES
		std::string RecoveredText;
		try{
			StringSource(EncryptedBlockText, true,
				new HexEncoder(
				new StreamTransformationFilter(decryptor,
				new StringSink(RecoveredText),
				BlockPaddingSchemeDef::NO_PADDING
				) // HexDecoder 
				) // StreamTransformationFilter
				); // StringSource*****/

			/****for (int j = 0; j < 16; j++)
				RecoveredText[j] ^= previousG[j];***/
		

		byte input[AES::BLOCKSIZE]; byte plain[AES::BLOCKSIZE];
		for (int l = i; l < i+AES::BLOCKSIZE; l++)
			input[l%AES::BLOCKSIZE] = cipherText[l];

		AES::Decryption alg2;
		alg2.SetKey((byte*)key.c_str(), 32);
		alg2.ProcessBlock(input, plain);

		/*for (int l = 0; l < AES::BLOCKSIZE; l++)
			std::cout << std::hex << (int)plain[l] << " ";*/
		std::cout << plain << std::endl;




			/*****PlainText += RecoveredText;
			std::cout << "\tkey:" << crypto.key << std::endl;
			std::cout << "recovered:" << RecoveredText << std::endl;*****/

		/*****}
		catch (Exception &e)		{			std::cout << e.what() << std::endl;		}****/
		/*****previousRecoveredText = RecoveredText;
		crypto.GetNextH();*****/

		/****for (int j = 0; j < EncryptedBlockText.length(); j++)
		{
			unsigned char c = EncryptedBlockText[j] ^ crypto.H[j];
			EncryptedBlockText[j] = c;
		}****/

		/**** crypto.GetNextG(previousRecoveredText); ****/
	}
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
	
	int numberOfFiles = (CryptoPP::AES::BLOCKSIZE + digits) / dataFileOperator.BundleSize()+1;
	std::cout << "blocksize:"<<CryptoPP::AES::BLOCKSIZE << "; " << "digits:"<<digits << "; " << "bundlesize:"<<dataFileOperator.BundleSize() << std::endl;
	filesContent = archiveLoader.GetFilesContent(numberOfFiles);

	std::cout << "loaded ciphertext:" << filesContent << std::endl;
	std::string len = filesContent.substr(0,digits);
	std::istringstream iss(len);
	int dl; iss >> dl;

	//reading initialization string IV

	for (int i = digits; i < digits + CryptoPP::AES::BLOCKSIZE; i++)
	{
		crypto.iv[i - digits] = filesContent[i];
	}
	//IV.push_back(file2.get());

	std::cout << "IV odtworzone:" << crypto.iv << " dl:" << sizeof crypto.iv << std::endl;
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

