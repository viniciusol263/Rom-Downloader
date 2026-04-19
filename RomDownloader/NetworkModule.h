#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <functional>

#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>

#include "ParserHandler.h"
#include "StorageModule.h"

using namespace winrt::Windows;
using namespace winrt::Windows::Web::Http;
using namespace winrt::Microsoft::UI::Xaml::Controls;


namespace Modules
{
	struct RomListing
	{
        RomListing()
		{
			romPack = "";
			romList = {};
		}

		RomListing(std::string pack, std::vector<std::pair<std::string, std::string>> list)
			: romPack(std::move(pack)), romList(std::move(list))
		{
		}
		std::string romPack;
		std::vector<std::pair<std::string, std::string>> romList;
	};

	class NetworkModule
	{
	public:
		NetworkModule();

		Foundation::IAsyncOperationWithProgress<Foundation::Collections::IMap<winrt::hstring, winrt::hstring>, int>  
			DownloadRoms(winrt::hstring romsToDownload, winrt::hstring region, winrt::hstring system, StorageFolder downloadPath, std::function<void(int)> progressCallback);
	private:
		HttpClient m_client;
		Handlers::ParserHelper m_parser;

		std::vector<std::pair<std::string, std::string>> m_urls;
		std::vector<RomListing> m_romsToDownload;

		Foundation::IAsyncAction GetUrlContentListing();
		Foundation::IAsyncOperationWithProgress<winrt::hstring, int> GetWebViewRequests(WebView2 const& webView);
		std::vector<std::pair<std::string, std::string>> ParseDOMContent(winrt::hstring const& content);
		Foundation::IAsyncOperation<winrt::hstring> StreamReadContent(IHttpContent const& content);
		std::vector<std::pair<std::string, std::string>>  RegexExtractLinks(std::vector<std::string> const& romList, std::string region, std::string system);
	};
}

