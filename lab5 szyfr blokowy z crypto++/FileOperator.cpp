#include "FileOperator.h"
#include <sstream>
#include "ArchiveLoader.h"

using namespace std;

FileOperator::FileOperator(string dir)
{
	directory = dir;
	GetFileStructure();
	//ListFileStructure();
}
FileOperator::FileOperator()
{
	directory = "";
	if (FirstLogon())
	{
		GetConfiguration();
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
}

void FileOperator::CreateArchiveDir()
{
	string archiveDir;
	string dd = this->directory;
	size_t poz = dd.find_last_of("\\");
	archiveDir ="\""+ dd.substr(0, poz + 1);
	archiveDir += "archive"; archiveDir += "\"";
	//cout << "Creating archive\n";
	string command = "mkdir " + archiveDir;

	system(command.c_str());
}

FileOperator::~FileOperator()
{
}
std::string FileOperator::GetArchiveDir() const
{
	string dir;
	size_t poz = this->directory.find_last_of("\\");
	dir = this->directory.substr(0, poz + 1);
	dir += "archive"; 
	return dir;
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
	return false;
}
void FileOperator::GetPath()
{
	//TODO:get from cin
	directory = "E:\\Studia\\Informatyka\\semestr VII\\oprogramowanie kryptograficzne\\projekt\\DANE";
	ofstream os("app.config");
	os << directory << endl;

}

void FileOperator::GetFileStructure()
{
	DIR *dp;
	FILE *fp;
	int i = 0;
	struct dirent *dirp;
	if ((dp = opendir(directory.c_str())) == NULL) 
	{
		cout << "Error(" << errno << ") opening " << directory << endl;
		//return errno;
	}

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
	string partialPath = pathDir.substr(directory.length()+1, partialPath.length())+'\\';//czêœæ œcie¿ki po œcie¿ce folderu z danymi
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
//TODO: dodaæ w pliku ca³¹ œcie¿kê
void FileOperator::ListFileStructure()
{
	cout << "struktura:" << endl;
	for (int i = 0; i < fileNames.size(); i++)
	{
		int depth = calculateDepth(fileNames[i]);
		for (int j = 0; j < depth; j++)
			cout << "\t";
		cout << fileNames[i] << endl;
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