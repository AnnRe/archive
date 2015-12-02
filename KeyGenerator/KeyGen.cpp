#include "KeyGen.h"

KeyGen::KeyGen(std::string id_str)
{
	id = ID(id_str);
}

KeyGen::~KeyGen()
{
}

std::string KeyGen::GenerateKey(std::string id)
{
	std::string sha256 = GetSHA256(id);
	std::string md5 = GetMD5(id);
	char letter;

	//first 5 chars
	std::string key1 = "";
	for (int i = 0; i < 5; i++)
	{
		letter = sha256[i];
		int j = 1;
		int num_letter = (int)(unsigned char)letter;
		while (!((letter >= 48 && letter <= 57) ||
			(letter >= 65 && letter <= 90) ||
			(letter >= 97 && letter < 122)
			))
		{
			num_letter += sha256[(i + j) % sha256.length()];
			num_letter %= 256;
			letter = (unsigned char)num_letter;
			j++;
		}
		key1.push_back(letter);
	}
	//next 5
	std::string key2 = "";
	for (int i = 0; i < 5; i++)
	{
		letter = md5[i];
		int j = 1;
		int num_letter = (int)(unsigned char)letter;
		while (!((letter >= 48 && letter <= 57) ||
			(letter >= 65 && letter <= 90) ||
			(letter >= 97 && letter < 122)
			))
		{
			num_letter -= sha256[(i + j) % sha256.length()];
			num_letter %= 256;
			letter = (unsigned char)num_letter;
			j++;
		}
		key2.push_back(letter);
	}
	//8 chars
	int mod[8] = { 1, 5, 6, 4, 7, 8, 2, 9 };
	std::string key3 = "";
	int i = 0;
	int j = 0;
	while (key3.length()<8)
	{
		int symb = (unsigned char)md5[i];

		int res = symb%mod[j % 8]; while (res < 0) res += mod[j];
		i = (i + 2) % md5.length();
		j++;
		unsigned char letter = 48 + res;
		key3.push_back(letter);

	}

	//key
	std::string KEY = key1 + '-' + key2 + '-' + key3;

	return KEY;

}

std::string KeyGen::GetMD5(std::string text)
{
	CryptoPP::MD5 md5;
	byte digest[CryptoPP::MD5::DIGESTSIZE];

	md5.CalculateDigest(digest, (byte*)text.c_str(), text.length());

	std::stringstream ss;
	ss << digest;
	return ss.str();

}
std::string KeyGen::GetSHA256(std::string text)
{
	CryptoPP::SHA256 sha;
	byte digest[CryptoPP::SHA256::DIGESTSIZE];

	sha.CalculateDigest(digest, (byte*)text.c_str(), text.length());

	std::stringstream ss;
	ss << digest;
	return ss.str();
}