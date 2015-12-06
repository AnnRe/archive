// C Runtime Includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include "ID.h"
#include "User.h"
#include "FileOperator.h"

// Includes all required Crypto++
// Block Cipher Headers
#include "SymmetricCipher.h"
#include "md5.h"
#include"aes.h"
// AutoSeededRandomPool
#include "osrng.h"

// Crypto++ Includes
#include "modes.h" // xxx_Mode< >
#include "filters.h" // StringSource and
#include "integer.h"
#include "FileEncrypter.h"
// StreamTransformation

// Cipher Modes
//
// #define CIPHER_MODE CBC_CTS_Mode
#define CIPHER_MODE CBC_Mode
// #define CIPHER_MODE CFB_FIPS_Mode
// #define CIPHER_MODE CFB_Mode
// #define CIPHER_MODE CTR_Mode
// #define CIPHER_MODE ECB_Mode
// #define CIPHER_MODE OFB_Mode

// Ciphers
//
#define CIPHER AES
// #define CIPHER Blowfish
// #define CIPHER BTEA
// #define CIPHER Camellia
// #define CIPHER CAST128
// #define CIPHER CAST256
// #define CIPHER DES
// #define CIPHER DES_EDE2
// #define CIPHER DES_EDE3
// #define CIPHER DES_XEX3
// #define CIPHER GOST
// #define CIPHER IDEA
// #define CIPHER MARS
// #define CIPHER RC2
// #define CIPHER RC5
// #define CIPHER RC6
// #define CIPHER Rijndael
// #define CIPHER SAFER_K
// #define CIPHER SAFER_SK
// #define CIPHER Serpent
// #define CIPHER SHACAL2
// #define CIPHER SHARK
// #define CIPHER SKIPJACK
// #define CIPHER ThreeWay
// #define CIPHER Twofish
// #define CIPHER XTEA

using namespace CryptoPP;
using namespace std;

int totalLength;

string int_to_string(int value, int length)
{
	ostringstream ss;
	ss << value;
	string size = ss.str();
	while (size.length() < length)//wyr�wnanie d�ugo�ci do 8
		size = "0" + size;
	return size;
}

void getNextKey(byte *key)
{
	const byte message[] = {*key};
	cout << "!!!!!!!!!" << sizeof(message) << "," << sizeof(*key) << endl;
	for (int i = 0; i < sizeof(message); i++)
		cout << i<<":" << message[i] << endl;
	SHA1 sha; // SHA1, SHA224, SHA256, SHA384, SHA512
	sha.Update(key, 5);
	int odpSize = sha.DigestSize();
	byte *odp = new byte[odpSize];
	sha.Final(odp);
	for (int i = 0; i < sizeof(key); i++)
		key[i] = (int)odp[i];
		//cout << hex <<  (int)odp[i];
	delete[] odp;
	//cout << endl;
}

int main(int argc, char* argv[]) {

	User user;
	bool accesAllowed = false; 
	if (user.licenceState==1)
	{
		cout << "ID zostalo wygenerowane (" << user.GenerateId() << ")" << endl;
		cout << "Podaj klucz: " << endl;
		std::string key;
		cin >> key;

		if (user.VerifyLicense(key))
		{
			std::cout << "Klucz poprawny" << std::endl;
			accesAllowed = true;
		}
		else
			std::cout << "Niepoprawny klucz" << std::endl;
	}
	else if (user.licenceState == 0)
	{
		cout << "Nie jestes zarejestrowany, twoje ID to:" << endl<<user.GenerateId()<<endl;
		
		cout << "Prze�lij go do dystrybutora, a otrzymasz klucz." << endl;
	} 
	else
	{
		user.ImportID();
		if (user.VerifyLicense())
		{
			cout << "Licencja poprawna" << endl;
			accesAllowed = true;
		}
		else
		{
			cout << "Niepoprawny klucz licencji" << endl;
			user.ClearKeyInFile();
		}
	}

	if (accesAllowed)
	{
		while(!user.MainPasswordCorrect())//TODO
			user.GetPassword();
		
		FileEncrypter encrypter;

		
		
	}

	/*string key;
	StringSource ss("28292A2B2D2E2F30323334353738393A3C3D3E3F41424344464748494B4C4D4E", true,
		new HexDecoder(
		new StringSink(key)));

	string plain;
	StringSource sc("D9DC4DBA3021B05D67C0518F72B62BF1", true,
		new HexDecoder(
		new StringSink(plain)));

	byte cipherData[AES::BLOCKSIZE];
	byte recoverData[AES::BLOCKSIZE];
	

	AES::Encryption alg1;
	alg1.SetKey((byte*)key.c_str(), 32);
	alg1.ProcessBlock((byte*)plain.c_str(), cipherData);

	ofstream ofs("cipher.txt");
	ofs << cipherData;
	ofs.close();

	for (int i = 0; i < AES::BLOCKSIZE; i++)
	{
		cout << hex << (int)cipherData[i] << " ";
	}
	cout << endl;

	AES::Decryption alg2;
	alg2.SetKey((byte*)key.c_str(), 32);
	alg2.ProcessBlock(cipherData, recoverData);
	ofstream ofr("recover.txt");
	ofr << cipherData;
	ofr.close();
	for (int i = 0; i < AES::BLOCKSIZE; i++)
		cout << hex << (int)recoverData[i] << " ";

	string stri;
	ifstream ifs("cipher.txt");
	byte cip[16];
	ifs >> stri;
	for (int i = 0; i < AES::BLOCKSIZE; i++)cip[i] = stri[i];
	ifs.close();

	alg2.ProcessBlock(cip, recoverData);
	for (int i = 0; i < AES::BLOCKSIZE; i++)
		cout << hex << (int)recoverData[i] << " ";*/

	system("PAUSE");

	return 0;
}
//*/

