#include "FileEncrypter.h"
#include "Encryptor.h"


FileEncrypter::FileEncrypter()
{
	Encryptor encryptor(fileOperator.totalLength,fileOperator);
	encryptor.SplitToFiles();
	encryptor.PrintModifiedFiles();
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
