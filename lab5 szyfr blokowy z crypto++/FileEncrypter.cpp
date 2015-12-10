#include "FileEncrypter.h"
#include "Encryptor.h"
#include "Decryptor.h"


FileEncrypter::FileEncrypter()
{
}
void FileEncrypter::Encrypt()
{
	Encryptor encryptor(fileOperator.totalLength, fileOperator, pass);

	encryptor.SplitToFiles();
	//encryptor.PrintModifiedFiles();
}
void FileEncrypter::Decrypt()
{
	Decryptor decryptor(fileOperator);
	decryptor.Run(pass);
}

void FileEncrypter::PrintStructure()
{
	fileOperator.ListFileStructure();
}

void FileEncrypter::PrintMenu()
{
	std::cout << "\n\n------------------------------------------------\n";
	std::cout << "\tMENU:" << std::endl;
	std::cout << "\t\t (P) - struktura plikow" << std::endl;
	std::cout << "\t\t (S) - szyfrowanie plikow" << std::endl;
	std::cout << "\t\t (D) - deszyfrowanie plikow" << std::endl;
	std::cout << "\t\t (O) - odswiezenie listy plikow\n" << std::endl;
	std::cout << "\t\t (Z) - zmiana hasla" << std::endl;
	std::cout << "\t\t (C) - zmiana sciezki do danych" << std::endl;

	//std::cout << "\t (Z) - Lista zmienionych plikow" << std::endl;
	std::cout << "------------------------------------------------\n" << std::endl;
}

void FileEncrypter::Refresh()
{
	fileOperator.GetFileStructure(true);
	fileOperator.ListFileStructure();
}

void FileEncrypter::ChangePath()
{
	fileOperator.ChangePath();
}

void FileEncrypter::GetPass(std::string _userPass)
{
	/*CryptoPP::StringSource ss("28292A2B2D2E2F30323334353738393A3C3D3E3F41424344464748494B4C4D4E", true,
		new CryptoPP::HexDecoder(
		new CryptoPP::StringSink(pass)));*/
	pass = _userPass;
}

FileEncrypter::~FileEncrypter()
{
}
