#pragma once
#include "ID.h"

static class KeyGen
{
public:
	KeyGen(std::string id);
	~KeyGen();	
	static std::string GenerateKey(std::string);

private:
	ID id;
	static std::string GetMD5(std::string text);
	static std::string GetSHA256(std::string text);
};

