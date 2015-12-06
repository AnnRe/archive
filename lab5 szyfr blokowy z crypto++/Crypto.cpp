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
void Crypto::GetNextG(std::string _previousCipherText)
{
	//std::cout << "---Getting next G ---\n";
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
	std::cout << "\nIV:" << iv << " dl:" << sizeof iv << std::endl;
	/*std::cout<< "pojedynczo:\n";
	for (int i = 0; i < sizeof iv; i++)
		std::cout << iv[i]<< std::endl;*/
}
