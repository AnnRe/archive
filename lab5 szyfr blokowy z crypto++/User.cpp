#include "User.h"


User::User()
{
	CheckLicense();
}


User::~User()
{
}

void User::ImportID()
{
	_licenseKey = id.ImportIDAndGetKeyFromFile();
}

void User::CheckLicense()
{
	std::ifstream file("license.txt");
	int fileLines = 0;
	std::string line;
	if (file.is_open())
	{

		while (!file.eof())//file exists
		{
			std::getline(file, line);
			fileLines++;
		}
		file.close();
		if (fileLines == 2)//only id generated
		{
			licenceState= 2;
		}
		else
		{
			licenceState= 1;
		}

	}
	else
	{
		//file not exist
		licenceState= 0;
	}
}

bool User::VerifyLicense(std::string licenseKey)
{
	std::string reconstructedKey = KeyGen::GenerateKey(id._number);
	if (reconstructedKey == licenseKey)
	{
		std::ofstream licenseFile;
		licenseFile.open("license.txt", std::ofstream::out | std::ofstream::app);
		licenseFile <<std::endl<< licenseKey ;
		licenseFile.close();
		return true;
	}
	else
		return false;
}
bool User::VerifyLicense()
{
	std::string reconstructedKey = KeyGen::GenerateKey(id._number);
	if (reconstructedKey == _licenseKey)
		return true;
	else
		return false;
}
std::string User::GenerateId()
{
	std::string Id = id.Get();

	std::ofstream licenseFile("license.txt");
	std::string ide = id.Get();
	licenseFile << ide ;
	licenseFile.close();

	return Id;
}

std::string User::ReadId()
{
	return id._number;
}

std::string User::ImportId()
{
	std::ofstream licenseFile("license.txt");

	std::string ide = id.Get();
	licenseFile << ide << std::endl;
	licenseFile.close();
	return ide;
}

void User::ClearKeyInFile()
{
	std::ofstream file("license.txt"); file.clear();

	file << id._number;
	file.close();
}

void User::GetPassword()
{

	//TODO: check if password already exists

	//if true
	std::cout << "Podaj has³o do aplikacji:";
	std::cin >> MainPassword;

	//if false
	//save password

	//calculate #
	//save # to file
}

bool User::MainPasswordCorrect()
{
	return true;//TODO: compare with file
}