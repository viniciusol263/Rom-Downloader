#include "pch.h"
#include <format>
#include <fstream>

#include "CloudModule.h"
#include "Constants.h"

#include <aws/s3/model/ListObjectsV2Request.h>
#include <aws/s3/model/GetObjectRequest.h>


namespace Modules
{
	CloudModule::CloudModule()
	{		
		Aws::SDKOptions options;
		Aws::InitAPI(options);

		m_credentials = std::make_shared<Aws::Auth::AWSCredentials>(Constants::AccessKey, Constants::SecretKey);
		m_config = std::make_shared<Aws::Client::ClientConfiguration>();
		m_config->region = region;

		m_client = std::make_shared<Aws::S3::S3Client>(*m_credentials, nullptr, *m_config);
	}

	CloudModule::~CloudModule()
	{
		Aws::SDKOptions options;
		Aws::ShutdownAPI(options);
	}

	std::vector<std::string> CloudModule::GetListing(std::string const& path)
	{
		std::vector<std::string> listing;
		Aws::S3::Model::ListObjectsV2Request request;
		request.SetBucket(s3BucketName);
		request.SetPrefix(path);

		auto outcome = m_client->ListObjectsV2(request);

		if (outcome.IsSuccess())
		{
			const auto& objects = outcome.GetResult().GetContents();
			for (const auto& object : objects)
			{
				if (object.GetKey() == path + "/") continue;
				listing.push_back(object.GetKey());
			}
		}
		
		return listing;
	}

	Foundation::IAsyncAction CloudModule::GetObject(std::string const& object, std::string const& downloadPath)
	{
		co_await winrt::resume_background();

		Aws::S3::Model::GetObjectRequest request;

		request.SetBucket(s3BucketName);
		request.SetKey(object);

		auto objectName = object.substr(object.find_last_of("/") + 1);

		auto outcome = m_client->GetObject(request);

		if (outcome.IsSuccess())
		{
			std::ofstream outputFile(downloadPath + "/" + objectName, std::ios::binary);

			outputFile << outcome.GetResult().GetBody().rdbuf();
		}
		
	}


}