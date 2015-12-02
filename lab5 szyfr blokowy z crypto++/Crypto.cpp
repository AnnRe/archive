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
	std::cout << "---\nGetting first G \n---\n";
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
	std::cout << "---\nGetting next AES key \n---\n";

	CryptoPP::SHA().CalculateDigest(key, key, sizeof(key));//generowanie i=tego klucza  //TODO:w³¹czyæ wszystkie 3 funkcje haszuj¹ce

}

void Crypto::GetNextH()
{
	std::cout << "---\nGetting next H \n---\n";
	CryptoPP::SHA256().CalculateDigest(H, key, sizeof(key));
}

void Crypto::GetNextG(std::string _previousCipherText)
{
	std::cout << "---\nGetting next G \n---\n";
	md5.Update((byte*)_previousCipherText.c_str(), sizeof(_previousCipherText));
	//md5.Update(licznik, sizeof(licznik));
	md5.Update(key, sizeof(key));
	md5.TruncatedFinal(G, CryptoPP::AES::BLOCKSIZE);
}

void Crypto::Initialize()
{
	std::cout << "-----\n Initializing ...." << std::endl;
	// Key and IV setup
	memset(iv, 0x01, CryptoPP::AES::BLOCKSIZE);
	prng.GenerateBlock(iv, sizeof(iv));
	prng.GenerateBlock(key, sizeof(iv));//TODO:zamieniæ na has³o u¿ytkownika
	std::cout << "IV:" << iv << std::endl;
}
