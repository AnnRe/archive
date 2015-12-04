#include "FileEncrypter.h"
#include "Encryptor.h"
#include "Decryptor.h"


FileEncrypter::FileEncrypter()
{
	Encryptor encryptor(fileOperator.totalLength,fileOperator);
	encryptor.SplitToFiles();
	//encryptor.PrintModifiedFiles();
	Decryptor decryptor(fileOperator);
	decryptor.Run();
	/*for (int i = 0; i < archiveFileOperator.fileNames.size(); i++)
	{
		std::string name = archiveFileOperator.fileNames[i];
		if (archiveFileOperator.fileTypes[name] == "DT_REG")
		{
			std::cout << "Szyfrowanie " << name << std::endl;
		}
	}*/
}


FileEncrypter::~FileEncrypter()
{
}
