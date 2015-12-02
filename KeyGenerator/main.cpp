// C Runtime Includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include "ID.h"


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
// StreamTransformation

// Cipher Modes
//
// #define CIPHER_MODE CBC_CTS_Mode
//#define CIPHER_MODE CBC_Mode
// #define CIPHER_MODE CFB_FIPS_Mode
// #define CIPHER_MODE CFB_Mode
// #define CIPHER_MODE CTR_Mode
// #define CIPHER_MODE ECB_Mode
// #define CIPHER_MODE OFB_Mode

// Ciphers
//
//#define CIPHER AES
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
#include "KeyGen.h"

int main(int argc, char* argv[]) {

	std::string id;
	cout << "Podaj id: ";
	cin >> id;
	std::string key = KeyGen::GenerateKey(id);
	cout << "Wygenerowany klucz:" << endl<<key <<endl;
	/*std::string x = id.Get();
	
	cout << x  << endl;*/

	system("PAUSE");
	return 0;
}
//*/

