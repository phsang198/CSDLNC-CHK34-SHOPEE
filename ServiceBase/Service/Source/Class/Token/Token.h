#pragma once

class authorize;
class token
{
public:
	token();
	~token() {};
public:
	void Clear();
	std::string Generate(Poco::DateTime startTime, long expireTime);
	std::string Renew(const std::string& jwt, Poco::DateTime startTime, long expireTime);
	BOOL getPayload(std::string jwt, std::map<std::string, std::string>& Payload);
	int Verify(std::string jwt);
	void setSecretKey(std::string _secretKey);
private:
	std::map<std::string, std::string> Header;
	std::map<std::string, std::string> Payload;
private:
	std::string secretKey;
};

class tokenManager
{
public:
	tokenManager();
	~tokenManager() {};

public:
	std::string secretKey = "";
	std::string host = "";

public:
	BOOL Encode(const std::string& jwt);
	int CheckAccessToken(const std::string& AccessToken);
	BOOL getListAutho(const std::string& AccessToken, std::map<std::string, authorize>& listAutho, std::string& productid);
	BOOL checkValidate(const std::string& AccessToken, std::string productid);
};