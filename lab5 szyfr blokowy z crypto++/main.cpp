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
using namespace CryptoPP;
using namespace std;


int main(int argc, char* argv[]) {

	User user;
	bool accesAllowed = false; 
	if (user.licenceState==1)
	{
		cout << "ID zostalo wygenerowane (" << user.GenerateId() << ")" << endl;
		cout << "Podaj klucz produktu: " << endl;
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
		
		cout << "Przeœlij go do dystrybutora, a otrzymasz klucz." << endl;
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
		bool login = false;
		if (user.licenceState==4)//user already set passwd
		{
			while (!user.MainPasswordCorrect())
				user.GetPassword();
			login = true;
		}
		else
			login=user.GetNewPassword();
		if (login)
		{
			system("cls");
			cout << "Logowanie pomyœlne"<<endl;
			
			FileEncrypter encrypter;
			cout << "Po encrypter" << endl;
			bool run = true; system("cls");
			while (run)
			{
				encrypter.PrintMenu();
				char choice;
				cin >> choice;
				switch (choice)
				{
				case 'P':case'p':
					encrypter.PrintStructure();
					break;
				case 'S':case's':
					encrypter.Encrypt();
					getchar();
					//system("cls");

					break; 
				case 'D':case'd':
						encrypter.Decrypt();
						//system("cls");
						break;

				default:
					run = false;
				}
			}
			encrypter.Encrypt();

		}

		
		
	}

	system("PAUSE");

	return 0;
}
//*/

