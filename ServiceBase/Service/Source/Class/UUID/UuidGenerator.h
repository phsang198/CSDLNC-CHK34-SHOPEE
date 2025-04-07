#pragma once
#include "uuid_4.h"


class UuidGenerator
{
public:
	UuidGenerator() 
	{
	}
	~UuidGenerator() {}
public:
	static std::string CreateUUID(); 
private:

};
