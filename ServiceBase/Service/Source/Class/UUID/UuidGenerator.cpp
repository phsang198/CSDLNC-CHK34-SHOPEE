#include "pch.h"
#include "UuidGenerator.h"


std::string UuidGenerator::CreateUUID()
{
	uuid4_init();

	char buf[UUID4_LEN];
	uuid4_generate(buf);
	return buf; 
}