#pragma once
#include<string>

#include <Windows.h>
#include <IPHlpApi.h>
#include<iostream>
#include<sstream>

#include "md5.h"
#include "sha.h"
#include"filters.h"
#include "hex.h"
#pragma comment(lib,"Iphlpapi.lib")
class ID
{
public:
	ID();
	ID(std::string id);
	ID(std::string id,std::string key);
	~ID();

	std::string Get();
	void Set(std::string);
	std::string value;
	int val;//value
	bool LicenseKeyCorrect(std::string ID, std::string Key);
private:
	std::string GetMacInfo(void);
	std::string GetMac(void);

	std::string GetSerialNumber(void);

	std::string GenerateId();
	std::string GetMD5(std::string text);
	std::string GetSHA256(std::string text);

	std::string GetIDPart1(std::string MAChash, std::string MAC);
	std::string GetIDPart2(std::string SerialNumber);

	char Letter(char symbol, char hashPart);//replace ascii sign with ascii -letter


};

