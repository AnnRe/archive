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

	CryptoPP::SHA().CalculateDigest(key, key, sizeof(key));//generowanie i=tego klucza  
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
	memset(iv, 0x01, CryptoPP::AES::BLOCKSIZE);
	prng.GenerateBlock(iv, sizeof(iv));
	//prng.GenerateBlock(key, sizeof(iv));//TODO:zamieniæ na has³o u¿ytkownika
	for (int i = 0; i < CryptoPP::AES::BLOCKSIZE; i++)
		key[i] = 0;
	std::cout << "\nIV:";
	for(int k=0;k< CryptoPP::AES::BLOCKSIZE;k++)
		std::cout<< iv[k] ;
	std::cout << std::endl;
	/*std::cout<< "pojedynczo:\n";
	for (int i = 0; i < sizeof iv; i++)
		std::cout << iv[i]<< std::endl;*/
}
