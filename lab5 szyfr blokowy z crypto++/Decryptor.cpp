#include "Decryptor.h"
#include "ArchiveLoader.h"
//#include <modes.h>
#include <hex.h>
#include<sstream>

using namespace CryptoPP;
Decryptor::Decryptor(FileOperator _fileOperator)
{
	archiveLoader = ArchiveLoader(_fileOperator.directory);
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
	std::cout << "----DESZYFROWANIE...\n Wejsciowy tekst ma dlugosc: " << cipherText.length() << std::endl;
	
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
		std::string x = cipherText;
		for (int l = i; l < i+AES::BLOCKSIZE; l++)
			encryptedTextBlock[l%AES::BLOCKSIZE] = (unsigned)cipherText[l];
		
		for (int j = 0; j < AES::BLOCKSIZE; j++) //H xor textblock
		{
			encryptedTextBlock[j] -= crypto.H[j];
			previousEncryptedTextBlock[j] = (unsigned)encryptedTextBlock[j];
		}
		//g1
		crypto.GetNextG(previousEncryptedTextBlock);
		// Decryption AES
		/*AES::Decryption alg2;
		alg2.SetKey((byte*)key.c_str(), 32);
		alg2.ProcessBlock(encryptedTextBlock, decryptedTextBlock);*/
		for (int j = 0; j < AES::BLOCKSIZE;j++)
			decryptedTextBlock[j] = encryptedTextBlock[j];

		//block ^G
		for (int j = 0; j < AES::BLOCKSIZE; j++)
		{
			decryptedTextBlock[j] +=crypto.Gprevious[j];
		}

		for (int k = 0; k < AES::BLOCKSIZE; k++)
			decryptedText.push_back(decryptedTextBlock[k]);
	}
	dataFileOperator.FilesText = decryptedText;
}
void Decryptor::SaveToFiles()//TODO:check unix/windows/mac
{
	struct stat info;
	std::string folderName = dataFileOperator.directory + "-backup";
	//Deleting old files and folders
	if (stat(folderName.c_str(), &info) == 0)
	{
		FileOperator fop(dataFileOperator.directory + "-backup",true);
		fop.DeleteDir();
	}

	//making new folder
	std::string comma = "mkdir  \"";
	std::string dirPath = dataFileOperator.directory + "-backup\""; comma += dirPath;
	system(comma.c_str());

	//recreating files
	int wsk = 0;
	std::string d = dataFileOperator.directory;
	for (int i = 0; i < dataFileOperator.fileNames.size();i++)
	if (dataFileOperator.fileTypes[dataFileOperator.fileNames[i]] == "DT_REG")
	{
		std::string fileContent = "";
		std::string x = dataFileOperator.FilesText;
			for (int j = 0; j < dataFileOperator.fileSizes[dataFileOperator.fileNames[i]]; j++)
			{
				fileContent.push_back(dataFileOperator.FilesText[wsk++]);
			}
		//saving tt txt file
		std::string nameWithoutExtension = FileOperator::GetName(dataFileOperator.fileNames[i]);
		std::string nametxt = nameWithoutExtension + ".txt";

		std::ofstream ofs(dataFileOperator.directory + "-backup\\" + nametxt, std::ios::binary);
		ofs << fileContent;
		ofs.close();

		//renaming txt file 
		std::string oldName = dataFileOperator.directory + "-backup\\" + nametxt;
		std::string newName = dataFileOperator.directory + "-backup\\" + dataFileOperator.fileNames[i];
		rename(oldName.c_str(), newName.c_str());

	}
	else
	{
		std::string command = "mkdir \"";
		std::string dirPat = dataFileOperator.directory + "-backup\\" + dataFileOperator.fileNames[i];
		command += dirPat; command += "\"";
		system(command.c_str());
	}
}

void Decryptor::Run()
{
	LoadConfiguration();
	LoadEncryptedFile();
	Decrypt();
	SaveToFiles();
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
	cipherText = CipherText.substr(poz+2);
}

