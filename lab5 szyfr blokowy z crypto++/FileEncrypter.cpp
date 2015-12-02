#include "FileEncrypter.h"
#include "Encryptor.h"
#include "Decryptor.h"


FileEncrypter::FileEncrypter()
{
	Encryptor encryptor(fileOperator.totalLength,fileOperator);
	encryptor.SplitToFiles();
	encryptor.PrintModifiedFiles();
	Decryptor decryptor(fileOperator);
	decryptor.LoadConfiguration();

	/*for (int i = 0; i < fileOperator.fileNames.size(); i++)
	{
		std::string name = fileOperator.fileNames[i];
		if (fileOperator.fileTypes[name] == "DT_REG")
		{
			std::cout << "Szyfrowanie " << name << std::endl;
		}
	}*/
}


FileEncrypter::~FileEncrypter()
{
}
