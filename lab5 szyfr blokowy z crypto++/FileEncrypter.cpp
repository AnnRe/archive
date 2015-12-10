#include "FileEncrypter.h"
#include "Encryptor.h"
#include "Decryptor.h"


FileEncrypter::FileEncrypter()
{
	//std::cout << "FileEncrypter" << std::endl;
	
}
void FileEncrypter::Encrypt()
{
	Encryptor encryptor(fileOperator.totalLength, fileOperator);
	encryptor.SplitToFiles();
	//encryptor.PrintModifiedFiles();
}
void FileEncrypter::Decrypt()
{
	Decryptor decryptor(fileOperator);
	decryptor.Run();
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
	std::cout << "\t\t (O) - odswiezenie listy plikow" << std::endl;
	//std::cout << "\t (Z) - Lista zmienionych plikow" << std::endl;
	std::cout << "------------------------------------------------\n" << std::endl;
}

void FileEncrypter::Refresh()
{
	fileOperator.GetFileStructure(true);
}

FileEncrypter::~FileEncrypter()
{
}
