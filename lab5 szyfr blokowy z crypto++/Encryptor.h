#pragma once
// C Runtime Includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>

// Includes all required Crypto++
// Block Cipher Headers
#include "SymmetricCipher.h"
#include "md5.h"
// AutoSeededRandomPool
#include "osrng.h"
// Crypto++ Includes
#include "modes.h" // xxx_Mode< >
#include "filters.h" // StringSource and
#include "integer.h"
#include "FileOperator.h"
// StreamTransformation

// Cipher Modes
#define CIPHER_MODE CBC_Mode

// Ciphers
#define CIPHER AES



class Encryptor
{
public:
	void PrintModifiedFiles();
	Encryptor(int totalLength,FileOperator _fileOperator);
	~Encryptor();
	static std::string int_to_string(int value, int length);
	void SplitToFiles();
private:
	//number of digits in size string
	int numberOfDigits = 8;
	std::string cipherText;
	FileOperator fileOperator;
	std::string PlainText;

	CryptoPP::AutoSeededRandomPool prng;
	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	byte iv[CryptoPP::AES::BLOCKSIZE];

	byte G[CryptoPP::MD5::DIGESTSIZE];
	byte H[CryptoPP::SHA256::DIGESTSIZE];

	CryptoPP::MD5 md5;

	void GetFirstG();
	void Run();
	void GetNextAESkey();
	void GetNextH();
	void GetNextG(std::string _previousCipherText);
	void Initialize();

	std::vector<std::string> modifiedFiles;
};

