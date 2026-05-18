#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <memory>

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/s3/S3Client.h>

#include <winrt/Windows.Foundation.h>

using namespace winrt::Windows;

namespace Modules
{
	const std::string s3BucketName = "romdownloader-s3-bucket-620830100933-us-east-2-an";
	const std::string region = "us-east-2";

	class CloudModule
	{
	public:
		CloudModule();
		~CloudModule();

		std::vector<std::string> GetListing(std::string const& path);
		Foundation::IAsyncAction GetObject(std::string const& object, std::string const& downloadPath);
		
	private:
		std::shared_ptr<Aws::Auth::AWSCredentials> m_credentials;
		std::shared_ptr<Aws::Client::ClientConfiguration> m_config;
		std::shared_ptr<Aws::S3::S3Client> m_client;
	};
}

