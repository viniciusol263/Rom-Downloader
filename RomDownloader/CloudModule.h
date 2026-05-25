#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <memory>

#include <winrt/Windows.Foundation.h>

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/s3/S3Client.h>



using namespace winrt::Windows;
namespace Aws::Lambda
{
	class LambdaClient;
}

namespace Modules
{
	const std::string lambdaUrlName = "arn:aws:lambda:us-east-2:620830100933:function:getRomDownloadURL";
	const std::string s3BucketName = "romdownloader-s3-bucket-620830100933-us-east-2-an";
	const std::string region = "us-east-2";

	class CloudModule
	{
	public:
		CloudModule();
		~CloudModule();

		std::vector<std::string> GetListing(std::string const& path);
		Foundation::IAsyncAction GetObject(std::string const& object, std::string const& downloadPath);
		Foundation::IAsyncOperation<winrt::hstring> InvokeLambda(Aws::String payload = "", std::string const& lambdaName = lambdaUrlName);
		
	private:
		std::shared_ptr<Aws::Auth::AWSCredentials> m_credentials;
		std::shared_ptr<Aws::Client::ClientConfiguration> m_config;
		std::shared_ptr<Aws::S3::S3Client> m_s3Client;
		std::shared_ptr<Aws::Lambda::LambdaClient> m_lambdaClient;
	};
}

