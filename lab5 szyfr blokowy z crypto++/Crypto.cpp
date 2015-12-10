#include "Crypto.h"
#include "Encryptor.h"


Crypto::Crypto()
{
}


Crypto::~Crypto()
{
}

void Crypto::GetFirstG()
{
	//std::cout << "---Getting first G \n---";
	//G0
	byte* licznik = nullptr;
	md5.Update(iv, sizeof(iv));
	/*cout << "sizeof licznik " << sizeof(licznik) << endl;
	md5.Update(licznik, sizeof(licznik));*/
	md5.Update(key, sizeof(key));
	md5.TruncatedFinal(G, CryptoPP::AES::BLOCKSIZE);
}
void Crypto::GetNextAESkey()
{
	//std::cout << "---Getting next AES key ---\n";
	std::string key_string = "";
	byte *digest=new byte[CryptoPP::SHA::DIGESTSIZE];
	CryptoPP::SHA().CalculateDigest(digest, key, CryptoPP::AES::DEFAULT_KEYLENGTH);//generowanie i=tego klucza  
	for (int i = 0; i < CryptoPP::AES::DEFAULT_KEYLENGTH; i++)
	{
		key[i] = digest[i];
	}
}
void Crypto::GetNextH()
{
	//std::cout << "---Getting next H ---\n";
	CryptoPP::SHA256().CalculateDigest(H, key, sizeof(key));
}
void Crypto::GetNextG(byte prev[])
{
	std::string _previousCipherText = "";
	for (int i = 0; i < CryptoPP::AES::BLOCKSIZE; i++)
		_previousCipherText.push_back(prev[i]);
	//std::cout << "---Getting next G ---\n";
	for (int i = 0; i < CryptoPP::MD5::DIGESTSIZE; i++)
		Gprevious[i] = G[i];
	md5.Update((byte*)_previousCipherText.c_str(), sizeof(_previousCipherText));
	//md5.Update(licznik, sizeof(licznik));
	md5.Update(key, sizeof(key));
	md5.TruncatedFinal(G, CryptoPP::AES::BLOCKSIZE);
}

void Crypto::Initialize()
{
	//std::cout << "Initializing ...." << std::endl;
	// Key and IV setup
	//memset(iv, 0x01, CryptoPP::AES::BLOCKSIZE);
	//prng.GenerateBlock(iv, sizeof(iv));//TODO: losowe
	for (int i = 0; i < CryptoPP::AES::BLOCKSIZE; i++)
		iv[i] = 'A'+i;
	
	//TODO:zamieniæ na has³o u¿ytkownika
	for (int i = 0; i < CryptoPP::AES::BLOCKSIZE; i++)
		key[i] = 0;
	std::cout << std::endl;
}
