#pragma once
#include "pch.h"
#include "Token.h"
#include "Authorization/Authorization.h"

token::token()
{
	Clear();

	Header["Algorithm"] = "HS256";
	Header["Type"] = "12565462";
	Header["Subject"] = "JWT";
}
void token::Clear()
{
	Header.clear();
	Payload.clear();
}
std::string token::Generate(Poco::DateTime startTime, long expireTime)
{
	Poco::JWT::Token token;

	token.setAlgorithm(Header["Algorithm"]);
	token.setType(Header["Type"]);
	token.setSubject(Header["Subject"]);

	/*Poco::Timestamp dtUTCNow = Poco::Timestamp::fromUtcTime((Poco::DateTime() + Poco::Timespan(10, 0)).utcTime());
	token.setExpiration(dtUTCNow);

	token.getIssuedAt(); */
	Poco::Timestamp dtStart = Poco::Timestamp::fromUtcTime((startTime.utcTime()));
	Poco::Timestamp dtExpire = Poco::Timestamp::fromUtcTime((startTime + Poco::Timespan(expireTime, 0)).utcTime());

	token.setIssuedAt(dtStart);
	token.setExpiration(dtExpire);
	token.setNotBefore(dtStart);

	for (auto x : Payload)
	{
		token.payload().set(x.first, x.second);
	}

	Poco::JWT::Signer signer(secretKey);
	std::string jwt = signer.sign(token, Poco::JWT::Signer::ALGO_HS256);

	return jwt;
}
std::string token::Renew(const std::string& jwt, Poco::DateTime startTime, long expireTime)
{
	Poco::JWT::Signer signer(secretKey);
	Poco::JWT::Token token = signer.verify(jwt);

	Poco::Timestamp dtStart = Poco::Timestamp::fromUtcTime((startTime.utcTime()));
	Poco::Timestamp dtExpire = Poco::Timestamp::fromUtcTime((startTime + Poco::Timespan(expireTime, 0)).utcTime());

	token.setIssuedAt(dtStart);
	token.setExpiration(dtExpire);
	token.setNotBefore(dtStart);

	std::string newJwt = signer.sign(token, Poco::JWT::Signer::ALGO_HS256);

	return newJwt;
}
BOOL token::getPayload(std::string jwt, std::map<std::string, std::string>& Payload)
{
	try
	{
		//Poco::JWT::Signer signer("123456");
		Poco::JWT::Signer signer(secretKey);
		Poco::JWT::Token token = signer.verify(jwt);

		Poco::JSON::Object tmp = token.payload();
		for (auto x : tmp)
		{
			std::string first = x.first;
			std::string second = x.second;

			Payload[first] = second;
		}

		return TRUE;
	}
	catch (std::exception e)
	{
		//CROW_LOG_ERROR << "Worker Crash: An uncaught exception occurred: " << e.what();
		return FALSE;
	}

	return TRUE;
}
int token::Verify(std::string jwt)
{
	try
	{
		//Poco::JWT::Signer signer("123456");
		Poco::JWT::Signer signer(secretKey);
		Poco::JWT::Token token = signer.verify(jwt);

		Poco::Timestamp dtExpiration = token.getExpiration();
		Poco::Timestamp NotBefore = token.getNotBefore();
		Poco::Timestamp dtUTCNow = Poco::Timestamp::fromUtcTime((Poco::DateTime().utcTime()));

		/*if (dtUTCNow < NotBefore)
		{
			return 105;
		}*/
		if (dtUTCNow > dtExpiration)
		{
			return 104;
		}
		return 200;
	}
	catch (std::exception e)
	{
		//CROW_LOG_ERROR << "Worker Crash: An uncaught exception occurred: " << e.what();
		return 106;
	}

	return 200;
}
void token::setSecretKey(std::string _secretKey)
{
	secretKey = _secretKey;
}
