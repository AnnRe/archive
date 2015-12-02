#pragma once
#include<string>
#include"ID.h"
#include "KeyGen.h"
#include <fstream>
class User
{
public:

	User();
	~User();
	
	std::string ImportId();
	std::string GenerateId();
	std::string ReadId();
	bool VerifyLicense(std::string licenseKey);
	bool VerifyLicense();
	void ImportID();
	void ClearKeyInFile();
	int licenceState;//0-nothing, 1-id generated, 2-registred
	void GetPassword();//gets main password do app
	bool MainPasswordCorrect();

private:
	bool hasLicense;
	std::string mainPasswordHash;
	std::string _licenseKey;
	ID id;
	void CheckLicense();
	std::string MainPassword;


};

