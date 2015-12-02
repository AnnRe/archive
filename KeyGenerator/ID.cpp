#include "ID.h"
#include "KeyGen.h"

ID::ID()
{
	

}

ID::ID(std::string id)
{
	value = id;
}

ID::ID(std::string val,std::string key)
{
	if (LicenseKeyCorrect(val, key))
	{
		Set(val);


	}

}
ID::~ID()
{
}

std::string ID::Get()
{
	std::string id= GenerateId();

	//std::string key=GenerateKey(id);
	//LicenseKeyCorrect(id, key);
	return id;
}

void ID::Set(std::string value)
{

}

std::string ID::GetMacInfo(void)
{
	std::string mac = "";
	std::stringstream ss;
	//wczytywanie numeru MAC
	IP_ADAPTER_INFO *info = NULL, *pos;
	DWORD size = 0;

	GetAdaptersInfo(info, &size);
	info = (IP_ADAPTER_INFO *)malloc(size);
	GetAdaptersInfo(info, &size);

	for (pos = info; pos != NULL; pos = pos->Next)
	{
		//std::cout << pos->Description << "\n";
		mac+=pos->Description;
		mac.push_back('\n');
		for (int i = 0; i < pos->AddressLength; i++)
		{
			ss << std::hex << (int)pos->Address[i] << ":";
		}
		mac += ss.str();
		mac.push_back('\n');
		ss.clear();
		//std::cout << "\n";
	}
	free(info);
	
	return mac;
}
std::string ID::GetMac(void)
{
	std::string mac = "";
	std::stringstream ss;
	//wczytywanie numeru MAC
	IP_ADAPTER_INFO *info = NULL, *pos;
	DWORD size = 0;

	GetAdaptersInfo(info, &size);
	info = (IP_ADAPTER_INFO *)malloc(size);
	GetAdaptersInfo(info, &size);

	for (pos = info; pos != NULL; pos = pos->Next)
	{
		for (int i = 0; i < pos->AddressLength; i++)
		{
			ss << std::hex << (int)pos->Address[i] << ":";
		}
		
		mac += ss.str();
		mac.pop_back();
		break;
	}
	free(info);
	return mac;
}

std::string ID::GetSerialNumber(void)
{
	DWORD dwSerial = 0;
	GetVolumeInformationA(NULL, NULL, NULL, &dwSerial, NULL, NULL, NULL, NULL);
	std::stringstream ss;
	ss << dwSerial;
	std::string num_str = ss.str();

	return num_str;
}

std::string ID::GenerateId()
{
	std::string id1,id2;
	std::string MAC = GetMac();
	std::string SerialNumber = GetSerialNumber();
	std::string MAC_hash = GetMD5(MAC);
	//std::string SerialNumber_hash = GetSHA256(SerialNumber);

	//std::cout << "key-part1: " << GetIDPart1(MAC_hash, MAC) << std::endl;
	id1 = GetIDPart1(MAC_hash, MAC);
	id2 = GetIDPart2(SerialNumber);
	std::string id = id1;
	id += id2;
	std::cout << "ID:" << id << std::endl;

	value= id;
	return id;
}

std::string ID::GetMD5(std::string text)
{
	CryptoPP::MD5 md5;
	byte digest[CryptoPP::MD5::DIGESTSIZE];
	
	md5.CalculateDigest(digest, (byte*)text.c_str(), text.length());

	std::stringstream ss;
	ss << digest;
	return ss.str();
		
}
std::string ID::GetSHA256(std::string text)
{
	CryptoPP::SHA256 sha;
	byte digest[CryptoPP::SHA256::DIGESTSIZE];

	sha.CalculateDigest(digest, (byte*)text.c_str(), text.length());

	std::stringstream ss;
	ss << digest;
	return ss.str();
}

std::string ID::GetIDPart1(std::string MAChash,std::string MAC)//******
{
	std::string key = "";
	//std::cout << MAC <<"  :"<<MAC.length()<< "\n";
	//std::cout << MAChash << "  :" << MAChash.length() << "\n";

	//std::cout << (unsigned int)(MAC[0]) << std::endl;
	for (int i = 0; i < 6; i++) //co 4-ty znak #MAC ^ co 3-ci znak MAC licz¹c od 2
	{							// ASCII 48-57, 65-90, 97-122
		char sign = MAChash[i * 3];
		//std::cout << "M: " << MAC[i * 3 + 1] << "(" << (int)(unsigned char)MAC[i * 3 + 1] << ") M#: " << MAChash[i * 4] << "(" << (int)(unsigned char)MAChash[i * 4]<<")"<< std::endl;
		sign ^= MAC[i * 3 + 1];
		key.push_back(Letter(MAC[i * 3 + 1], MAChash[i * 3]));
	}

	return key;
}


std::string ID::GetIDPart2(std::string SerialNumber)//******
{
	std::cout << "seryjny: " << SerialNumber<<std::endl;
	std::string value = "";
	int i = SerialNumber.length() - 1;
	while (value.length() < 5)
		value.push_back(SerialNumber[i--]);
	return value;
}

char ID::Letter(char symbol, char hashPart)//symbol from alphabet change into another using symbol from hash function
{
	//ASCII 48 - 57, 65 - 90, 97 - 122 : 59
	char new_symbol = symbol;
	int move = ((int)hashPart) % 60;
	if (move <0) move += 60;
	//std::cout << "Move: " << move << " symbol: " << symbol<<"("<<(int)symbol<<") ";
	while (move)
	{
		if (new_symbol >= 48 && new_symbol < 57)
			new_symbol++;
		else if (new_symbol == 57)
			new_symbol = (char)65;
		else if (new_symbol >= 65 && new_symbol < 90)
			new_symbol++;
		else if (new_symbol == 90)
			new_symbol = (char)97;
		else if (new_symbol >= 97 && new_symbol < 122)
			new_symbol++;
		else if (new_symbol == 122)
			new_symbol = (char)48;
		move--;
	}
	//std::cout << "new_symbol: " << new_symbol << "(" << (int)new_symbol << ") " << std::endl;;

	return new_symbol;
}


bool ID::LicenseKeyCorrect(std::string ID, std::string Key)
{
	std::string reconstructed_key = KeyGen::GenerateKey(ID);
	for (int i = 0; i < reconstructed_key.length(); i++)
	{
		if (Key[i] != reconstructed_key[i])
			return false;
	}
	return true;
}