#pragma once
#include "pch.h"
#include "S3Wrapper.h"

// aws.
#include <aws/core/auth/AWSCredentials.h>
#include <aws/core/utils/UUID.h>
#include <aws/core/utils/StringUtils.h>
#include <aws/s3/model/Owner.h>
#include <aws/s3/model/Grant.h>
#include <aws/s3/model/Grantee.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/s3/model/DeleteBucketRequest.h>
#include <aws/s3/model/BucketLocationConstraint.h>
#include <aws/s3/model/GetBucketLocationRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/ListObjectsRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/AccessControlPolicy.h>
#include <aws/s3/model/PutBucketAclRequest.h>
#include <aws/s3/model/GetBucketAclRequest.h>
#include <aws/s3/model/GetObjectAclRequest.h>
#include <aws/s3/model/PutObjectAclRequest.h>
#include <aws/s3/model/PutObjectAclRequest.h>


// stl
#include <fstream>
#include <iterator>


/*===================================================================================================================
========================================== CS3Wrapper ===========================================================
===================================================================================================================*/


CS3Wrapper::CS3Wrapper()
{
	Aws::InitAPI(_awsOptions);
	_pS3Client = nullptr;
}

CS3Wrapper::~CS3Wrapper()
{
	Aws::ShutdownAPI(_awsOptions);
}

void CS3Wrapper::InitAS3API(const std::string& szBucket, const std::string& szRegion)
{
	_pS3Client = NewS3Client(_szHost, _szAccessKeyID, _szSecretAccessKey, szRegion);
	if(_pS3Client)
	{
		_szBucket = Aws::Utils::StringUtils::ToLower(szBucket.c_str());
		_szRegion = Aws::Utils::StringUtils::ToLower(szRegion.c_str());
	}
}

void CS3Wrapper::ShutdownAS3API()
{
}

std::shared_ptr<Aws::S3::S3Client> CS3Wrapper::NewS3Client(const std::string& szHost, const std::string& szAccessKeyID, const std::string& szSecretAccessKey, const std::string& szRegion)
{
	Aws::Client::ClientConfiguration clConfig;
	if (!szRegion.empty())
		clConfig.region = szRegion;
	clConfig.endpointOverride = szHost;
	clConfig.scheme = Aws::Http::Scheme::HTTP;
	clConfig.verifySSL = false;
	Aws::Auth::AWSCredentials credentials(szAccessKeyID, szSecretAccessKey);
	return std::make_shared<Aws::S3::S3Client>(credentials, clConfig, Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never, false);
}

bool CS3Wrapper::FindTheBucket(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket)
{
	if (pS3Client == nullptr)
		return false;

	Aws::S3::Model::ListBucketsOutcome lstOutCome = pS3Client->ListBuckets();
	if (lstOutCome.IsSuccess())
	{
		Aws::Vector<Aws::S3::Model::Bucket> vBuckets = lstOutCome.GetResult().GetBuckets();
		for (Aws::S3::Model::Bucket const& bucket : vBuckets)
		{
			if (bucket.GetName() == szBucket)
				return true;
		}
	}

	return false;
}

bool CS3Wrapper::CreateTheBucket(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket, const std::string& szRegion)
{
	if (pS3Client == nullptr)
		return false;

	Aws::S3::Model::CreateBucketConfiguration creConfig;
	creConfig.SetLocationConstraint(Aws::S3::Model::BucketLocationConstraintMapper::GetBucketLocationConstraintForName(szRegion));
	Aws::S3::Model::CreateBucketRequest request;
	request.SetBucket(szBucket);
	request.SetCreateBucketConfiguration(creConfig);
	auto outcome = pS3Client->CreateBucket(request);

	return outcome.IsSuccess();
}

bool CS3Wrapper::DeleteTheBucket(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket)
{
	if (pS3Client == nullptr)
		return false;

	Aws::S3::Model::DeleteBucketRequest requestDelete;
	requestDelete.SetBucket(szBucket);
	auto ocDelete = pS3Client->DeleteBucket(requestDelete);

	return ocDelete.IsSuccess();
}

bool CS3Wrapper::PutObjectFile(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket, const std::string& szKey, const std::string& szFilePath)
{
	Aws::S3::Model::PutObjectRequest request;
	request.SetBucket(szBucket);
	request.SetKey(szKey);

	std::shared_ptr<Aws::IOStream> input_data = Aws::MakeShared<Aws::FStream>("pbf", szFilePath.c_str(), std::ios_base::in | std::ios_base::binary);
	request.SetBody(input_data);
	request.SetContentType(Aws::Utils::StringUtils::ToLower("application/octet-stream"));

	auto ocPutObj = pS3Client->PutObject(request);
	return ocPutObj.IsSuccess();

}

bool CS3Wrapper::PutObjectData(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket, const std::string& szKey, const std::string& szData)
{
	Aws::S3::Model::PutObjectRequest request;
	request.SetBucket(szBucket);
	request.SetKey(szKey);

	std::shared_ptr<Aws::IOStream> input_data = Aws::MakeShared<Aws::StringStream>("");

	*input_data << szData;
	if (!input_data->good())
		return false;

	long nLength = static_cast<long>(szData.size());
	request.SetBody(input_data);
	request.SetContentType("application/octet-stream");
	request.SetContentEncoding("gzip");
	request.SetContentLength(nLength);

	Aws::S3::Model::PutObjectOutcome outcome = pS3Client->PutObject(request);

	if (!outcome.IsSuccess()) 
	{
		printf("Error: PutObjectBuffer: ,%s\n", outcome.GetError().GetMessage()); 

		return false;
	}
	else
	{
		printf("Success: uploaded to bucket \n") ;

		return true;
	}
}
bool CS3Wrapper::ListObjects(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket, const std::string& szKey,std::vector<std::string>& listObjectKey)
{
	Aws::S3::Model::ListObjectsRequest request;
	request.WithBucket(szBucket);
	request.WithPrefix(szKey);
	//request.w(szKey);

	auto outcome = pS3Client->ListObjects(request);

	if (outcome.IsSuccess())
	{
		printf("Objects in bucket '%s' \n" , szBucket);

		Aws::Vector<Aws::S3::Model::Object> objects =
			outcome.GetResult().GetContents();
		
		if (objects.empty()) return false; 

		for (Aws::S3::Model::Object& object : objects)
		{
			listObjectKey.push_back(object.GetKey());
		}

		return true;
	}
	else
	{
		printf("Error: ListObjects: %s\n",outcome.GetError().GetMessage()) ;

		return false;
	}
}

bool CS3Wrapper::GetObject(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket, const std::string& szKey, std::string& res)
{
	Aws::S3::Model::GetObjectRequest request;
	request.SetBucket(szBucket);
	request.SetKey(szKey);

	Aws::S3::Model::GetObjectOutcome get_object_outcome = pS3Client->GetObject(request);

	if (get_object_outcome.IsSuccess())
	{
		auto& retrieved_file = get_object_outcome.GetResultWithOwnership().GetBody();

		// Print a beginning portion of the text file.
		Aws::StringStream ss;
		ss << retrieved_file.rdbuf();
		res = ss.str(); 
		return true;
	}
	else
	{
		auto err = get_object_outcome.GetError();
		printf("Error: GetObject: %s : %s \n", err.GetExceptionName(), err.GetMessage()); 

		return false;
	}
}

bool CS3Wrapper::DeleteObject(std::shared_ptr<Aws::S3::S3Client> pS3Client, const std::string& szBucket, const std::string& szKey)
{
	if (pS3Client == nullptr)
		return false;

	Aws::S3::Model::DeleteObjectRequest requestDelete;
	requestDelete.WithKey(szKey).WithBucket(szBucket);
	auto ocDelete = pS3Client->DeleteObject(requestDelete);

	return ocDelete.IsSuccess();
}
