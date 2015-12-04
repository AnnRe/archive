#include "Encryptor.h"
#include <hex.h>
using namespace CryptoPP;
using namespace std;

void Encryptor::Run()
{
	// Cipher Text
	string BlockCipherText;
	string PreviousBlockCipherText;

	//GetFirstG();
	crypto.GetFirstG();

	cout << "----SZYFROWANIE...\n";
	for (int i = 0; i < PlainText.size(); i += AES::BLOCKSIZE)//rozbicie na bloki
	{
		crypto.GetNextAESkey();

		string BlockText;
		BlockText = PlainText.substr(i, i + AES::BLOCKSIZE);
		while (BlockText.size() < AES::BLOCKSIZE)
			BlockText.push_back('x');
		//cout << "===\n blok tekstu:" << BlockText << endl << "===\n";
		// Encryptor
		CBC_Mode<AES>::Encryption
			Encryptor(crypto.key, sizeof(crypto.key), crypto.iv);//CBC AES

		for (int j = 0; j < AES::BLOCKSIZE; j++) //G xor textblock
			BlockText[j] = (unsigned)BlockText[j] ^ crypto.G[j];

		// Encryption AES
		StringSource(BlockText.c_str(), true,
			new HexEncoder(
			new StreamTransformationFilter(Encryptor,
			new StringSink(BlockCipherText)
			) // HexEncoder
			) // StreamTransformationFilter
			); // StringSource
		//cout << "po AES:\n" << BlockCipherText << endl;

		PreviousBlockCipherText = BlockCipherText;

		crypto.GetNextH();

		//H1 xor AES output
		for (int j = 0; j < BlockCipherText.length(); j++)
		{
			unsigned char c = BlockCipherText[j] ^ crypto.H[j];
			BlockCipherText[j] = c;
		}
		cipherText += BlockCipherText;
		//Gi+1=MD5( output z AES, key, licznik);
		crypto.GetNextG(PreviousBlockCipherText);

	}
	cout << "----Koniec szyfrowania...\n";

}

void Encryptor::Initialize()
{
	crypto.Initialize();

	string sizeString = int_to_string(PlainText.length(), numberOfDigits);

	cipherText = sizeString;
	// Configuration to ciphertext
	stringstream ss;
	ss << crypto.iv;
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
		PlainText = "Ala ma kota, a kot ma Al�.";
		Initialize();
		Run();
		
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
	while (size.length() < length)//wyr�wnanie d�ugo�ci do 8
		size = "0" + size;
	return size;
}

void Encryptor::SplitToFiles()
{
	cout << "Splitting to files ... " << endl;
	
	cout << "Cipher text (encryptor):" << cipherText<<endl;
	int bundleSize = fileOperator.BundleSize();
	int bundleNumber = 1;
	fileOperator.CreateArchiveDir();
	string archiveDir=fileOperator.GetArchiveDir();

	for (int i = 0; i < cipherText.length(); i+=bundleSize)
	{
		string fileContent="";
		for (int j = i; j < i+bundleSize; j++)
			fileContent.push_back(cipherText[j]);

		string fileName = archiveDir+"\\bundle";
		stringstream ss; ss << bundleNumber++; fileName += ss.str();
		fileName += ".txt";

		ofstream os(fileName);
		os << fileContent;
		os.close();

		modifiedFiles.push_back(fileName.substr(8,fileName.length()));

	}

}
//TODO:
// poprawi� hashe
// deszyfrowanie
