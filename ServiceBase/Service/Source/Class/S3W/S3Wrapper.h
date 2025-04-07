#pragma once
#ifndef S3_WRAPPER_H
#define S3_WRAPPER_H

// aws.
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>


/*===================================================================================================================
============================================== CS3Wrapper ===========================================================
===================================================================================================================*/

class CS3Wrapper
{
protected:
	std::string	_szHost;
	std::string	_szAccessKeyID;
	std::string	_szSecretAccessKey;
	std::string	_szBucket;
	std::string	_szRegion;

	Aws::SDKOptions						_awsOptions;

public:
	std::shared_ptr<Aws::S3::S3Client>	_pS3Client;

public:
	CS3Wrapper();
	virtual ~CS3Wrapper();

public:
	void SetHost(const std::string& szHost) { _szHost = szHost; }
	void SetAccessKeyID(const std::string& szAccessKeyID) { _szAccessKeyID = szAccessKeyID; }
	void SetSecretAccessKey(const std::string& szSecretAccessKey) { _szSecretAccessKey = szSecretAccessKey; }
	void SetBucket(const std::string& szBucket) { _szBucket = szBucket; }
	void SetRegion(const std::string& szRegion) { _szRegion = szRegion; }

	const std::string& GetHost() { return _szHost; }
	const std::string& GetAccessKeyID() { return _szAccessKeyID; }
	const std::string& GetSecretAccessKey() { return _szSecretAccessKey; }
	const std::string& GetBucket() { return _szBucket; }
	const std::string& GetRegion() { return _szRegion; }

	void InitAS3API(const std::string& szBucket, const std::string& szRegion);
	void ShutdownAS3API();

	static std::shared_ptr<Aws::S3::S3Client> NewS3Client(const std::string& szHost, const std::string& szAccessKeyID, const std::string& szSecretAccessKey, const std::string& szRegion);
	static bool FindTheBucket(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket);
	static bool CreateTheBucket(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket, const std::string& szRegion);
	static bool DeleteTheBucket(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket);
	static bool PutObjectFile(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket, const std::string& szKey, const std::string& szFilePath);
	static bool PutObjectData(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket, const std::string& szKey, const std::string& szData);
	static bool ListObjects(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket, const std::string& szKey, std::vector<std::string>& listObjectKey);
	static bool GetObject(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket, const std::string& szKey, std::string& res);
	static bool DeleteObject(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket, const std::string& szKey);
};

#endif // S3_WRAPPER_H