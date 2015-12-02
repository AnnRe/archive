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
#include "Crypto.h"
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
	
	void Run();
	void Initialize();

	std::vector<std::string> modifiedFiles;

	Crypto crypto;
};

