#include "Encryptor.h"
#include <hex.h>
using namespace CryptoPP;
using namespace std;

void Encryptor::Run()
{
	// Cipher Text

	//GetFirstG();
	crypto.GetFirstG();
	cout << "----SZYFROWANIE...\n";
	for (int i = 0; i < PlainText.size(); i+=AES::BLOCKSIZE)//rozbicie na bloki
	{
		string BlockCipherText;
		string PreviousBlockCipherText;

		crypto.GetNextAESkey();
		/*****string BlockText;
		for (int j = i; j < i+16; j++)
		{
			if (j < PlainText.size())
				BlockText.push_back(PlainText[j]);
			else
				BlockText.push_back('x');
		}*****/

		byte blockText[AES::BLOCKSIZE];
		for (int j = i; j < i + AES::BLOCKSIZE; j++)
		{
			if (j < PlainText.size())
				blockText[j%AES::BLOCKSIZE] = PlainText[j];
			else 
				blockText[j%AES::BLOCKSIZE] = 'x';
		}

		/****cout << "===\n blok tekstu:" << BlockText << endl << "===("<<BlockText.length()<<")\n";*****/
		cout << "===\n blok tekstu:" << blockText << endl << "===("<<sizeof blockText<<")\n";

		string key;
		StringSource ss("28292A2B2D2E2F30323334353738393A3C3D3E3F41424344464748494B4C4D4E", true,
			new HexDecoder(
			new StringSink(key)));

		byte cipherData[AES::BLOCKSIZE];

		cout << "plain:\n" << blockText << endl;

		AES::Encryption alg1;
		alg1.SetKey((byte*)key.c_str(), 32);
		alg1.ProcessBlock(blockText, cipherData);
		// Encryptor
		cout << "cipher:\n" << cipherData << endl;
		/*CTR_Mode<AES>::Encryption
			Encryptor(crypto.key, sizeof(crypto.key), crypto.iv);//CTR AES*/
		/****for (int j = 0; j < AES::BLOCKSIZE; j++) //G xor textblock
			BlockText[j] = (unsigned)BlockText[j] ^ crypto.G[j];****/

		// Encryption AES
		/****StringSource(BlockText, true,
			new HexEncoder(
			new StreamTransformationFilter(Encryptor,
			new StringSink(BlockCipherText),
			BlockPaddingSchemeDef::NO_PADDING
			) // HexEncoder
			) // StreamTransformationFilter
			); // StringSource*/
		
		/*****cout << "\tAES key:" << crypto.key << endl;
		cout << "po AES:\n" << BlockCipherText << "\n("<<BlockCipherText.length()<<")"<<endl;*****/

		PreviousBlockCipherText = BlockCipherText;

		crypto.GetNextH();

		//H1 xor AES output
		/****for (int j = 0; j < BlockCipherText.length(); j++)
		{
			unsigned char c = BlockCipherText[j] ^ crypto.H[j];
			BlockCipherText[j] = c;
		}****/
		/****cout << "out:" << BlockCipherText.length() << endl;
		cipherText += BlockCipherText;*****/
		//Gi+1=MD5( output z AES, key, licznik);
		crypto.GetNextG(PreviousBlockCipherText);

		ofstream ofs("outu.txt");
		ofs << cipherData;
		ofs.close();
		for (int j = 0; j < AES::BLOCKSIZE; j++)
			cipherText.push_back(cipherData[j]);

	}
	cout << cipherText << endl;
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
		PlainText = "Alla ma kota, a kot ma Alee i cos jeszcze tam dalej.";
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
	while (size.length() < length)//wyrównanie d³ugoœci do 8
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
// poprawiæ hashe
// deszyfrowanie
