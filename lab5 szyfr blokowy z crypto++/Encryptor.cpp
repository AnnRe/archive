#include "Encryptor.h"
#include <hex.h>
using namespace CryptoPP;
using namespace std;

void Encryptor::Run()
{
	// Cipher Text

	crypto.GetFirstG();
	cout << "----SZYFROWANIE...\n";
	cout << "---\n *************** \n"<<cipherText <<"\n *************** \n"<< endl;
	for (int i = 0; i < PlainText.size(); i+=AES::BLOCKSIZE)//rozbicie na bloki
	{
		string BlockCipherText;
		byte previousCipherData[AES::BLOCKSIZE];

		crypto.GetNextAESkey();
		
		byte blockText[AES::BLOCKSIZE];
		for (int j = i; j < i + AES::BLOCKSIZE; j++)
		{
			if (j < PlainText.size())
			{
				blockText[j%AES::BLOCKSIZE] = PlainText[j];
			}
			else
			{
				blockText[j%AES::BLOCKSIZE] = 'x';
			}
		}
		
		string key;
		StringSource ss("28292A2B2D2E2F30323334353738393A3C3D3E3F41424344464748494B4C4D4E", true,
			new HexDecoder(
			new StringSink(key)));
		//block ^G
		for (int j = 0; j < AES::BLOCKSIZE; j++)
		{
			blockText[j] -= crypto.G[j];
		}

		byte cipherData[AES::BLOCKSIZE];

		//AES Encryption
		/*AES::Encryption alg1;
		alg1.SetKey((byte*)key.c_str(), 32);
		alg1.ProcessBlock(blockText, cipherData);*/
		for (int j = 0; j < AES::BLOCKSIZE; j++)
		{
			cipherData[j] = blockText[j];
		}
		// Encryptor
		
		for (int k = 0; k < AES::BLOCKSIZE; k++)
			previousCipherData[k] = (unsigned)cipherData[k];
		//g1
		crypto.GetNextG(previousCipherData);
		crypto.GetNextH();
		//H1 xor AES output
		for (int j = 0; j < AES::BLOCKSIZE; j++)
		{
			//cipherData[j] +=crypto.H[j];
		}

		for (int j = 0; j < AES::BLOCKSIZE; j++)
			cipherText.push_back((unsigned)cipherData[j]);

	}
	cout << "----Koniec szyfrowania...\n wyjsciowy tekst ma dl:"<<cipherText.length()<<endl;
	cout << cipherText << endl;

}

void Encryptor::Initialize()
{
	crypto.Initialize();

	string sizeString = int_to_string(PlainText.length(), numberOfDigits);

	cipherText = sizeString;
	// Configuration to ciphertext
	for (int i = 0; i < AES::BLOCKSIZE;i++)
		cipherText.push_back(crypto.iv[i]);
	string x = cipherText;
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
		PlainText = fileOperator.GetTotalContent();
		string x = PlainText;
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

		ofstream os(fileName, std::ios_base::binary);
		os << fileContent;
		os.close();

		modifiedFiles.push_back(fileName.substr(8,fileName.length()));

	}

}
