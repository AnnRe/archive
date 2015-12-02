#include "Encryptor.h"
using namespace CryptoPP;
using namespace std;

void Encryptor::GetFirstG()
{
	cout << "---\nGetting first G \n---\n";
	//G0
	byte* licznik = nullptr;
	md5.Update(iv, sizeof(iv));
	/*cout << "sizeof licznik " << sizeof(licznik) << endl;
	md5.Update(licznik, sizeof(licznik));*/
	md5.Update(key, sizeof(key));
	md5.TruncatedFinal(G, CIPHER::BLOCKSIZE);
}

void Encryptor::Run()
{
	// Cipher Text
	string BlockCipherText;
	string PreviousBlockCipherText;

	GetFirstG();

	cout << "\n----\nSZYFROWANIE...\n";
	for (int i = 0; i < PlainText.size(); i += CIPHER::BLOCKSIZE)//rozbicie na bloki
	{
		GetNextAESkey();
		string BlockText;
		BlockText = PlainText.substr(i, i + CIPHER::BLOCKSIZE);
		while (BlockText.size() < CIPHER::BLOCKSIZE)
			BlockText.push_back('x');
		cout << "===\n blok tekstu:" << BlockText << endl << "===\n";
		// Encryptor
		CIPHER_MODE<CIPHER>::Encryption
			Encryptor(key, sizeof(key), iv);//CBC AES

		for (int j = 0; j < CIPHER::BLOCKSIZE; j++) //G xor textblock
			BlockText[j] = (unsigned)BlockText[j] ^ G[j];

		// Encryption AES
		StringSource(BlockText, true,
			new StreamTransformationFilter(Encryptor,
			new StringSink(BlockCipherText)
			) // StreamTransformationFilter
			); // StringSource
		cout << "po AES:\n" << BlockCipherText << endl;

		PreviousBlockCipherText = BlockCipherText;

		GetNextH();

		//H1 xor AES output
		for (int j = 0; j < BlockCipherText.length(); j++)
		{
			unsigned char c = BlockCipherText[j] ^ H[j];
			BlockCipherText[j] = c;
		}
		//file << BlockCipherText;
		cipherText += BlockCipherText;
		//Gi+1=MD5( output z AES, key, licznik);
		GetNextG(PreviousBlockCipherText);

	}
	cout << "----\nKoniec szyfrowania...\n";

}

void Encryptor::GetNextAESkey()
{
	cout << "---\nGetting next AES key \n---\n";

	SHA().CalculateDigest(key, key, sizeof(key));//generowanie i=tego klucza  //TODO:w³¹czyæ wszystkie 3 funkcje haszuj¹ce

}

void Encryptor::GetNextH()
{
	cout << "---\nGetting next H \n---\n";
	SHA256().CalculateDigest(H, key, sizeof(key));
}

void Encryptor::GetNextG(string _PreviousCipherText)
{
	cout << "---\nGetting next G \n---\n";
	md5.Update((byte*)_PreviousCipherText.c_str(), sizeof(_PreviousCipherText));
	//md5.Update(licznik, sizeof(licznik));
	md5.Update(key, sizeof(key));
	md5.TruncatedFinal(G, CIPHER::BLOCKSIZE);
}
void Encryptor::Initialize()
{
	cout << "-----\n Initializing ...." << endl;
	// Key and IV setup
	memset(iv, 0x01, CIPHER::BLOCKSIZE);
	prng.GenerateBlock(iv, sizeof(iv));
	prng.GenerateBlock(key, sizeof(iv));//TODO:zamieniæ na has³o u¿ytkownika
	cout << "IV:" << iv << endl;

	string sizeString = int_to_string(PlainText.length(), numberOfDigits);

	cipherText = sizeString;
	// Configuration to ciphertext
	stringstream ss;
	ss << iv;
	cipherText+= ss.str();
}

void Encryptor::PrintModifiedFiles()
{
	cout << "Zmodyfikowane paczki:" << endl;
	for (int i = 0; i < modifiedFiles.size(); i++)
		cout << modifiedFiles[i] << endl;
}

Encryptor::Encryptor(int totalLength, FileOperator _fileOperator)
{
	fileOperator = _fileOperator;
	try
	{
		// Message M
		PlainText = "Ala ma kota, a kot ma Alê.";
		Initialize();

		Run();

		///////////////////////////////////////
		
		// Recovered Text 
		string RecoveredText;

		// Decryptor
		/*CIPHER_MODE<CIPHER>::Decryption
		Decryptor(key, sizeof(key), iv);

		// Decryption
		StringSource(CipherText, true,
		new StreamTransformationFilter(Decryptor,
		new StringSink(RecoveredText)
		) // StreamTransformationFilter
		); // StringSource

		//////////////////////////////////////////
		// Output //
		//////////////////////////////////////////

		cout << "Algorithm:" << endl;
		cout << " " << Encryptor.AlgorithmName() << endl;
		cout << "Minimum Key Size:" << endl;
		cout << " " << Encryptor.MinKeyLength() << " bytes" << endl;
		cout << endl;

		cout << "Plain Text (" << PlainText.length() << " bytes)" << endl;
		cout << " '" << PlainText << "'" << endl;
		cout << endl;

		cout << "Cipher Text Size:" << endl;
		cout << " " << CipherText.size() << " bytes" << endl;
		cout << endl;

		cout << "Recovered Text:" << endl;
		cout << " '" << RecoveredText << "'" << endl;
		cout << endl;*/
	}
	catch (Exception& e)
	{
		cerr << e.what() << endl;
	}
	catch (...)
	{
		cerr << "Unknown Error" << endl;
	}
}


Encryptor::~Encryptor()
{
}




string Encryptor::int_to_string(int value, int length)
{
	ostringstream ss;
	ss << value;
	string size = ss.str();
	while (size.length() < length)//wyrównanie d³ugoœci do 8
		size = "0" + size;
	return size;
}

void Encryptor::SplitToFiles()
{
	cout << "Splitting to files ... " << endl;
	int bundleSize = fileOperator.BundleSize();
	int bundleNumber = 1;
	FileOperator::CreateArchiveDir();

	for (int i = 0; i < cipherText.length(); i+=bundleSize)
	{
		string fileContent;
		for (int j = i; j < i+bundleSize; j++)
			fileContent.push_back(cipherText[i*bundleSize + j]);

		string fileName = "archive\\bundle";
		stringstream ss; ss << bundleNumber++; fileName += ss.str();
		fileName += ".txt";
		cout << fileName << endl;

		ofstream os(fileName);
		os << fileContent;
		os.close();

		modifiedFiles.push_back(fileName.substr(8,fileName.length()));

	}

}
//TODO:
// poprawiæ hashe
// deszyfrowanie
// po szyfrowaniu podzia³ na pliki
