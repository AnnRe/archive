// C Runtime Includes
#include <iostream>
#include <cstdlib>
#include <string>
#include "User.h"

// Includes all required Crypto++
// Block Cipher Headers
#include "md5.h"
// AutoSeededRandomPool

// Crypto++ Includes
#include "FileEncrypter.h"
// StreamTransformation


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
		cout << "\n\tNie jestes zarejestrowany, twoje ID to: " << endl<<user.GenerateId()<<endl;
		
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
			encrypter.GetPass(user.MainPassword);
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
					system("cls");

					break; 
				case 'D':case'd':
						encrypter.Decrypt();
						getchar();
						system("cls");
						break;
				case 'O':case'o':
					encrypter.Refresh();
					getchar();
					system("cls");
					break;
				case 'Z':case'z':
					user.ChangePassword();
					break;
				case'C':case'c':
					encrypter.ChangePath();
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

