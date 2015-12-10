#include "FileOperator.h"
#include <sstream>
#include "ArchiveLoader.h"

using namespace std;

FileOperator::FileOperator(string dir,bool archive)
{
	directory = dir;
	GetFileStructure();

	if (!archive)
		SaveFileStructure("structure.txt");
	//ListFileStructure();
}

bool FileOperator::StructureFileExists()
{
	struct stat info;
	std::string fileName = "structure.txt";
	//Deleting old files and folders
	if (stat(fileName.c_str(), &info) == 0)//file exists
	{
		return true;
	}
	return false;
}

FileOperator::FileOperator()
{
	directory = "";
	if (FirstLogon()||!StructureFileExists())
	{
		if (!ConfigurationSet())
			GetConfiguration();
		else
		{
			LoadPath();
			LoadBundleSize();
		}
		GetFileStructure();
		//ListFileStructure();
		SaveFileStructure("structure.txt");
		GetTotalLength();
	}
	else
	{
		LoadConfigurationAndStructure();
		//ListFileStructure();
		GetTotalLength();
	}
}

void FileOperator::LoadSizeStringSize()//druga linia pliku appConfig
{
	ifstream is("app.config");
	string line;

	for (int i = 0; i < 3; i++)
	{
		getline(is, line);
	}
	istringstream iss(line);
	iss>>numberOfDigits;
}

void FileOperator::LoadConfigurationAndStructure()
{
	LoadPath();
	LoadBundleSize();
	LoadSizeStringSize();
	LoadFileStructure("structure.txt");
}

void FileOperator::GetTotalLength()
{
	totalLength = 0;
	for (int i = 0; i < fileNames.size(); i++)
	{
		//cout << "+" << fileNames[i] << "(" << fileSizes[fileNames[i]] << ")\n";
		if (fileSizes[fileNames[i]]>0)
			totalLength += fileSizes[fileNames[i]];
	}
	//cout << "total length:" << totalLength << endl;
}

void FileOperator::GetConfiguration()
{
	GetPath();
	GetBundleSize();

	ofstream os("app.config");
	os << directory << endl;
	os << bundleSize;
	os.close();
}

void FileOperator::CreateArchiveDir()
{
	//clearing old archive
	string archiveDir;
	string dd = this->directory;
	size_t poz = dd.find_last_of("\\");
	archiveDir ="\""+ dd.substr(0, poz + 1);
	archiveDir += "archive"; 
	//cout << "Creating archive\n";
	string clearDirCommand = "del /Q " + archiveDir + "\\*\"";
	system(clearDirCommand.c_str());
	archiveDir += "\"";
	string deleteCommand = "rmdir /Q " + archiveDir;
	system(deleteCommand.c_str());

	//creating new archive
	string command = "mkdir " + archiveDir;

	system(command.c_str());
}

FileOperator::~FileOperator()
{
}

void FileOperator::DeleteDir()
{
	ListFileStructure();
	if (fileNames.size() > 0)
	{
		for (int i = 0; i < fileNames.size(); i++)//deleting files in all folders
		{
			if (fileTypes[fileNames[i]] == "DT_REG")
			{
				std::string path = "\"" + directory + "\\" + fileNames[i] + "\"";
				std::string command = "del " + path;
				system(command.c_str());
			}
		}
		for (int i = 0; i < fileNames.size(); i++)
		{
			int j = fileNames.size() - i - 1;
			if (fileTypes[fileNames[j]] == "DT_DIR")
			{
				std::string path = "\"" + directory + "\\" + fileNames[j] + "\"";
				std::string command = "rmdir \q " + path;
				system(command.c_str());
			}
		}
	}
	std::string path = "\"" + directory + "\"";
	std::string command = "rmdir \q " + path;
	system(command.c_str());

}

std::string FileOperator::GetName(std::string fileNameWithExtension)
{
	size_t poz = fileNameWithExtension.find_last_of('.');
	std::string toReturn = fileNameWithExtension.substr(0, poz);
	return toReturn;
}

std::string FileOperator::GetArchiveDir() const
{
	string dir;
	size_t poz = this->directory.find_last_of("\\");
	dir = this->directory.substr(0, poz + 1);
	dir += "archive"; 
	return dir;
}


std::string FileOperator::GetTotalContent()
{
	std::string total="";
	for (int i = 0; i < fileNames.size();i++)
	if (fileTypes[fileNames[i]] == "DT_REG")
	{
		std::string content=ArchiveLoader::FileContent(directory+"\\"+fileNames[i]);
		total+=content;
	}
	return total;
}

int FileOperator::NumberOfFiles()
{
	int n = 0;
	for (int i = 0; i < fileNames.size();i++)
	if (fileTypes[fileNames[i]] == "DT_REG")
		n++;
	return n;
}

int FileOperator::BundleSize()
{
	return bundleSize;
}

bool FileOperator::FirstLogon()
{
	ifstream conf("app.config");
	int n = 0;
	string line;
	while (!conf.eof())
	{
		getline(conf, line);
		n++;
	}
	conf.close();
	return n<1;
}
void FileOperator::GetPath()
{
	//TODO:get from cin
	std::string path; char x;
	struct stat info;
	std::cout << "Podaj sciezke folderu danych : "; cin.ignore();
	getline(cin, path);
	//Deleting old files and folders
	while (stat(path.c_str(), &info) != 0)
	{	
		std::cout << "Podaj poprawna sciezke folderu danych: "; cin.ignore();
		getline(cin, path);
	}
	directory = path;

}

void FileOperator::GetFileStructure()
{
	fileNames.clear();
	DIR *dp;
	FILE *fp;
	int i = 0;
	struct dirent *dirp;
	if ((dp = opendir(directory.c_str())) == NULL) 
	{
		cout << "Error(" << errno << ") opening " << directory << endl;
		//return errno;
	}
	cout << "dir:" << directory << endl;
	while ((dirp = readdir(dp)) != NULL) 
	{
		string name = dirp->d_name;

		if (dirp->d_type == DT_REG)
		{
			fileNames.push_back(name);
			string path = directory + "\\" + name;
			
			int chars_n = CountChars(path);
			//cout <<"plik: "<< name << ", " << /*(unsigned int)file_stats.st_size*/chars_n << endl;
			fileSizes[dirp->d_name] = chars_n;
			//cout << "\t ***"<<chars_n << endl;
			fileTypes[dirp->d_name] = "DT_REG";//regular file
		}
		else if (dirp->d_type == DT_DIR )
		{
			//cout << "Katalog: "<<name << endl;
			string path = directory + "\\" + name;
			if (name != "." && name != "..")
			{
				fileNames.push_back(string(dirp->d_name));
				fileSizes[dirp->d_name] = -1;
				fileTypes[dirp->d_name] = "DT_DIR";
				GetFileStructure(path);
			}
		}
		else
		{
			//cout << "inny typ " << dirp->d_name << endl;
		}
	}
	closedir(dp);
}
void FileOperator::GetFileStructure(std::string pathDir)
{
	DIR *dp;
	FILE *fp;
	int i = 0;
	struct dirent *dirp;
	if ((dp = opendir(pathDir.c_str())) == NULL)
	{
		cout << "Error(" << errno << ") opening " << pathDir << endl;
		//return errno;
	}
	string partialPath = pathDir.substr(directory.length()+1, partialPath.length());//cz�� �cie�ki po �cie�ce folderu z danymi
	while ((dirp = readdir(dp)) != NULL)
	{
		string name = dirp->d_name; 
		if (dirp->d_type == DT_REG)
		{
			fileNames.push_back(partialPath+"\\"+name);
			string path = pathDir + "\\" + name;

			int chars_n = CountChars(path);
			//cout << "plik: " << name << ", " << /*(unsigned int)file_stats.st_size*/chars_n << endl;
			fileSizes[partialPath + "\\" + name] = chars_n;
			fileTypes[partialPath + "\\" + name] = "DT_REG";//regular file

		}
		else if (dirp->d_type == DT_DIR)
		{
			//cout << "Katalog: " << name << endl;
			string path = pathDir + "\\" + name;
			if (name != "." && name != "..")
			{
				fileNames.push_back(partialPath+"\\"+name);
				fileSizes[partialPath + "\\" + name] = -1;
				fileTypes[partialPath + "\\" + name] = "DT_DIR";
				GetFileStructure(path);
			}
		}
		else
		{
			//cout << "inny typ " << dirp->d_name << endl;
		}
	}
	closedir(dp);
}
void FileOperator::ListFileStructure()
{
	//LoadConfigurationAndStructure();
	system("cls");
	GetFileStructure();
	cout << "\n\n\tSTRUKTURA:" << endl;
	if (fileNames.size() > 0)
	{
		for (int i = 0; i < fileNames.size(); i++)
		{
			int depth = calculateDepth(fileNames[i]);
			for (int j = 0; j < depth; j++)
				cout << "   ";

			cout << fileNames[i] << endl;
		}
	}
	else
	{
		cout << "Brak plikow" << endl;
	}

}

int FileOperator::CountChars(string path)
{
	ifstream file(path);
	int n = 0;
	while (!file.eof())
	{
		file.get();
		n++;
	}
	file.close();
	return n;

}

void FileOperator::SaveFileStructure(std::string fileName)
{
	ofstream os(fileName);

	for (int i = 0; i < fileNames.size(); i++)
	{
		string name = fileNames[i];
		os << name << " " << fileTypes[name] << " " << fileSizes[name];
		os << endl;
	
	}
	os.close();

}

int FileOperator::calculateDepth(std::string filePath)
{
	int depth = 0;
	for (int i = 0; i < filePath.length(); i++)
	if (filePath[i] == '\\')
		depth++;
	return depth;
}

void FileOperator::GetBundleSize()
{
	cout << "Podaj wielkosc paczek (B): ";//TODO:validation
	cin>>bundleSize;
}
void FileOperator::LoadPath()
{
	try
	{
		ifstream is("app.config");
		string line;
		getline(is, line);
		directory = line;

	}
	catch (...)
	{
		cout << "! Nie moge otworzyc pliku konfiguracji!" << endl;
	}

}
void FileOperator::LoadBundleSize()
{
	try
	{
		ifstream is("app.config");
		string line;
		getline(is, line);
		getline(is, line);

		istringstream ss(line);
		ss >> bundleSize;

	}
	catch (...)
	{
		cout << "! Nie moge otworzyc pliku konfiguracji!" << endl;
	}

}

bool FileOperator::ConfigurationSet()
{
	struct stat info;
	std::string fileName = "app.config";
	//Deleting old files and folders
	if (stat(fileName.c_str(), &info) == 0)//file exists
	{
		std::string line;
		int counter = 0;
		ifstream iss("app.config");
		while (!iss.eof())
		{
			getline(iss, line);
			counter++;
		}	
		iss.close();
		if (counter == 2)
			return true;
	}
	return false;

}

void FileOperator::LoadFileStructure(std::string fileName)
{
	ifstream is("structure.txt");		//name type size
	while (!is.eof())
	{
		string line;
		getline(is, line);
		if (!line.empty())
		{
			size_t pos1 = line.find_first_of(" ");

			string name = line.substr(0, pos1);
			fileNames.push_back(name);
			line = line.substr(pos1 + 1, line.length());

			size_t pos2 = line.find_first_of(" ");
			string type = line.substr(0, pos2);
			fileTypes[name] = type;

			string size = line.substr(pos2, name.length() - 1);
			istringstream iss(size);
			int fileSize;
			iss >> fileSize;
			fileSizes[name] = fileSize;
		}
	}
	is.close();
}
