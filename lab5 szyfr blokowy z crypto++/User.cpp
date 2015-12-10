#include "User.h"


User::User()
{
	CheckLicenseState();
}


User::~User()
{
}

void User::ImportID()
{
	_licenseKey = id.ImportIDAndGetKeyFromFile();
}

void User::CheckLicenseState()
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
		
		switch (fileLines)
		{
		case 2:
			licenceState = 2;
			break;
		case 1://only ID generated
			licenceState = 1;
			break;
		case 0://empty file
			licenceState = 0;
			break;
		case 3://Id generated, key saved,
			licenceState = 3;
			break;
		default://Id generated, key saved, passwd set
			licenceState = 4;
			break;
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

bool User::GetNewPassword()
{
	system("cls");
	std::cout << "\n\tUSTAL HASLO GLOWNE:\n" << std::endl;
	std::string pass;
	bool state = false;
	while (true)
	{
		std::cout << "Podaj haslo glowne: ";
		 std::cin >> pass;
		std::cout << "Powtorz haslo: ";
		std::string pass2; std::cin >> pass2;
		if (pass == pass2)
		{
			system("cls");
			std::cout << "Hasla zgodne.\n";
			state = true;
			break;
		}
	}
	SavePasswd(pass);
	return state;
}

bool User::HasSetPasswd()
{
	std::ifstream ifs("license.txt");
	int n = 0;
	std::string line;
	while (!ifs.eof())
	{
		std::getline(ifs, line);
		n++;
	}
	ifs.close();
	if (n == 4)
	{
		if (line.length() == CryptoPP::SHA1::DIGESTSIZE)
			return true;
	}
	return false;
}

void User::SaveNewPasswd(std::string newPasswd)
{
	MainPassword = newPasswd;

	//saving user passwd # to license file
	std::ofstream of;
	std::string line, other;
	std::ifstream iff("license.txt");

	getline(iff, line);//license id
	other = line;
	getline(iff, line);//license key
	other += '\n' + line;
	iff.close();

	of.open("license.txt", std::ofstream::out);
	of << other << std::endl;
	std::string hash = GetSha(MainPassword);
	of << hash;
	of.close();
	std::cout << "Haslo zmienione " << std::endl;
	std::getchar();

}

void User::ChangePassword()
{
	system("cls");
	std::cout << "\n\tZMIANA HASLA GLOWNEGO:\n" << std::endl;
	std::string newPass, oldPass;
	bool state = false;
	while (true)
	{
		std::cout << "Podaj stare haslo: ";
		std::cin >> oldPass;
		if (oldPass == MainPassword)
			break;
	}
	while (true)
	{
		std::cout << "Podaj nowe haslo glowne: ";
		std::cin >> newPass;
		std::cout << "Powtorz nowe haslo: ";
		std::string pass2; std::cin >> pass2;
		if (newPass == pass2)
		{
			system("cls");
			std::cout << "Hasla zgodne.\n";
			state = true;
			break;
		}
	}
	SaveNewPasswd(newPass);
}

void User::ClearKeyInFile()
{
	std::ofstream file("license.txt"); file.clear();

	file << id._number;
	file.close();
}

void User::SavePasswd(std::string mainPasswd)
{
	MainPassword = mainPasswd;

	//saving user passwd # to license file
	std::ofstream of;
	of.open("license.txt", std::ofstream::out | std::ofstream::app); 
	of << std::endl;
	std::string hash= GetSha(MainPassword);
	of << hash;
	of.close();


}

void User::GetPassword()
{

	std::cout << "Podaj haslo do aplikacji:";
	std::cin >> MainPassword;
}

bool User::MainPasswordCorrect()
{
	std::ifstream ifs("license.txt");
	std::string line;
	for (int i = 0; i <2; i++)
		std::getline(ifs, line);
	std::string sha = "";
	char ch;
	while (!ifs.eof())
	{
		ifs.get(ch);
		sha.push_back(ch);
	}
	ifs.close();
	std::string givenPasswdHash = GetSha(MainPassword);
	if (sha.substr(0,15) == givenPasswdHash.substr(0,15))
		return true;
	return false;
}

std::string User::GetSha(std::string passwd)
{
	CryptoPP::SHA1 sha; // SHA1, SHA224, SHA256, SHA384, SHA512
	sha.Update((byte*)passwd.c_str(), 5);
	int odpSize = sha.DigestSize();
	byte *odp = new byte[odpSize];
	sha.Final(odp);

	std::string tmp = "";
	for (int i = 0; i < CryptoPP::SHA1::DIGESTSIZE; i++)
		tmp.push_back(odp[i]);
	return tmp;
}