#pragma once
#include <cryptlib.h>
#include <aes.h>
#include <sha.h>
#include <md5.h>
#include <osrng.h>

class Crypto
{
public:
	Crypto();
	~Crypto();
	
	void GetFirstG();
	void GetNextAESkey();
	void GetNextH();
	void GetNextG(byte prev[]);

	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	byte iv[CryptoPP::AES::BLOCKSIZE];

	byte G[CryptoPP::MD5::DIGESTSIZE];
	byte H[CryptoPP::SHA256::DIGESTSIZE];
	byte Gprevious[CryptoPP::MD5::DIGESTSIZE];
	void Initialize();

private:
	CryptoPP::AutoSeededRandomPool prng;
	CryptoPP::MD5 md5;



};

