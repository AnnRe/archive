#include "Decryptor.h"
#include "ArchiveLoader.h"
#include <modes.h>
#include <hex.h>

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
	std::string previousRecoveredText;
	std::string PlainText;
	crypto.GetFirstG();
	std::cout << "----DESZYFROWANIE...\n";

	int blockSize = AES::BLOCKSIZE;
	for (int i = 0; i < cipherText.size(); i += blockSize)
	{
		crypto.GetNextAESkey();

		std::string EncryptedBlockText = cipherText.substr(i, i + blockSize);
		std::cout << "Block:" << EncryptedBlockText <<"(len="<<EncryptedBlockText.length()<<")"<< std::endl;
		while (EncryptedBlockText.size() < AES::BLOCKSIZE)
			EncryptedBlockText.push_back('x');
		//Decryptor
		CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption
			Decryptor(crypto.key, sizeof(crypto.key), crypto.iv);
		for (int j = 0; j < AES::BLOCKSIZE; j++) //G xor textblock
			EncryptedBlockText[j] = (unsigned)EncryptedBlockText[j] ^ crypto.G[j];


		// Decryption AES
		std::string RecoveredText;
		try{
			StringSource(EncryptedBlockText, true,
			new CryptoPP::HexDecoder(
			new StreamTransformationFilter(Decryptor,
			new StringSink(RecoveredText)
			) // HexDecoder 
			) // StreamTransformationFilter
			); // StringSource
}
catch (Exception &e)
{
	std::cout << e.what()<<std::endl;
}
		previousRecoveredText = RecoveredText;

		crypto.GetNextH();

		for (int j = 0; j < EncryptedBlockText.length(); j++)
		{
			unsigned char c = EncryptedBlockText[j] ^ crypto.H[j];
			EncryptedBlockText[j] = c;
		}

		PlainText += EncryptedBlockText;

		crypto.GetNextG(previousRecoveredText);
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

