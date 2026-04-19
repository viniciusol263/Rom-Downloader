#include "pch.h"
#include "NetworkModule.h"
#include "Constants.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Web.Http.Headers.h>
#include <winrt/Windows.Web.Http.Filters.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <gumbo.h>
#include <nlohmann/json.hpp>
#include <string_view>
#include <regex>
#include <format>
#include <algorithm>


using json = nlohmann::json;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Streams;

namespace Modules
{
	NetworkModule::NetworkModule()
	{
		Filters::HttpBaseProtocolFilter filter;
		filter.AutomaticDecompression(true);

		m_client = HttpClient(filter);
		m_client.DefaultRequestHeaders().UserAgent().ParseAdd(
			L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/147.0.0.0 Safari/537.36");
		m_client.DefaultRequestHeaders().Connection().TryParseAdd(L"keep-alive");
		m_client.DefaultRequestHeaders().Accept().ParseAdd(
			L"text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
		m_client.DefaultRequestHeaders().AcceptLanguage().ParseAdd(
			L"pt-BR,pt;q=0.9,en-US;q=0.8,en;q=0.7");
		m_client.DefaultRequestHeaders().AcceptEncoding().Clear();
		m_client.DefaultRequestHeaders().CacheControl().ParseAdd(L"max-age=0");

		m_urls = Constants::SystemsUrl;
		GetUrlContentListing();
	}

	Foundation::IAsyncOperationWithProgress<Foundation::Collections::IMap<winrt::hstring, winrt::hstring>,int> 
		NetworkModule::DownloadRoms(winrt::hstring romsToDownload, winrt::hstring region, winrt::hstring system, StorageFolder downloadPath, std::function<void(int)> progressCallback)
	{
		auto _romsToDownload = winrt::to_string(romsToDownload);
		std::string_view romsToDownloadView{ _romsToDownload };

		auto romList = m_parser.GetRomList(romsToDownloadView);
		auto downloadLinks = RegexExtractLinks(romList, winrt::to_string(region), winrt::to_string(system));
		auto map = winrt::single_threaded_map<winrt::hstring, winrt::hstring>();

		for (auto const& link : downloadLinks)
		{
			auto unescapedUri = winrt::to_string(Foundation::Uri::UnescapeComponent(winrt::to_hstring(link.second)));
			auto key = Foundation::Uri::UnescapeComponent(
				winrt::to_hstring(link.second)
			);
			auto value = winrt::to_hstring(link.first);
			map.Insert(key, value);
			auto response = co_await m_client.GetAsync(Foundation::Uri(winrt::to_hstring(link.second)), HttpCompletionOption::ResponseHeadersRead);
			StorageFile file = co_await downloadPath.CreateFileAsync(winrt::to_hstring(unescapedUri.substr(unescapedUri.find_last_of("/") + 1)), CreationCollisionOption::ReplaceExisting);
			
			response.EnsureSuccessStatusCode();
			auto length = response.Content().Headers().ContentLength();
			if (length)
			{
				uint64_t size = length.Value();
			}
			else {
				progressCallback(200);
			}

			IInputStream inputStream = co_await response.Content().ReadAsInputStreamAsync();
			IRandomAccessStream fileStream = co_await file.OpenAsync(FileAccessMode::ReadWrite);

			co_await RandomAccessStream::CopyAsync(inputStream, fileStream);
			co_await fileStream.FlushAsync();
		}

		progressCallback(0);

		co_return map;
	}


	Foundation::IAsyncAction NetworkModule::GetUrlContentListing()
	{	
		for (auto& url : m_urls)
		{
			auto uri = Foundation::Uri(winrt::to_hstring(url.second));
			auto response = co_await m_client.GetAsync(uri, HttpCompletionOption::ResponseHeadersRead);
			if (!response.IsSuccessStatusCode()) continue;

			auto content = co_await StreamReadContent(response.Content());
			auto parsedContent = ParseDOMContent(content);
			m_romsToDownload.emplace_back(url.first, parsedContent);
		}
	}

	Foundation::IAsyncOperationWithProgress<winrt::hstring, int> NetworkModule::GetWebViewRequests(WebView2 const& webView)
	{
		winrt::hstring result;
		for (auto& url : m_urls)
		{
			winrt::apartment_context ui;
			co_await webView.EnsureCoreWebView2Async();

			winrt::handle eventHandle{ CreateEvent(nullptr, true, false, nullptr) };

			winrt::event_token token;
			token = webView.NavigationCompleted([&](auto const&, auto const& args)
			{
				webView.NavigationCompleted(token);
				SetEvent(eventHandle.get());
			});

			webView.Source(Foundation::Uri(winrt::to_hstring(url.second)));


			co_await winrt::resume_on_signal(eventHandle.get());

			ResetEvent(eventHandle.get());

			co_await ui;
			token = webView.NavigationCompleted([&](auto const&, auto const& args)
				{
					webView.NavigationCompleted(token);
				
					SetEvent(eventHandle.get());
				});

			webView.Reload();

			co_await winrt::resume_on_signal(eventHandle.get());

			co_await ui;
			co_await webView.ExecuteScriptAsync(
				LR"(new Promise(resolve => {
					if (document.readyState === 'complete') resolve();
					else window.addEventListener('load', () => resolve());
					}))"
			);


			co_await ui;
			auto response = co_await webView.ExecuteScriptAsync(LR"(document.documentElement.outerHTML)");
			
			std::string raw = winrt::to_string(response);

			std::string html = json::parse(raw).get<std::string>();

			result = winrt::to_hstring(html);

		}
		co_return result;
	}


	std::vector<std::pair<std::string, std::string>> NetworkModule::ParseDOMContent(winrt::hstring const& content)
	{
		m_parser.ParseDOMContent(winrt::to_string(content));
		return m_parser.GetLinks();
	}

	Foundation::IAsyncOperation<winrt::hstring> NetworkModule::StreamReadContent(IHttpContent const& content)
	{
		IInputStream stream = co_await content.ReadAsInputStreamAsync();
		DataReader reader(stream);

		reader.InputStreamOptions(InputStreamOptions::Partial);

		std::string html;
		constexpr uint32_t chunckSize = 64 * 1024;

		while (true)
		{
			uint32_t bytesLoaded = co_await reader.LoadAsync(chunckSize);

			if (bytesLoaded == 0) break;

			auto oldSize = html.size();
			html.resize(oldSize + bytesLoaded);
		
			reader.ReadBytes(winrt::array_view<uint8_t>(reinterpret_cast<uint8_t*>(&html[oldSize]), reinterpret_cast<uint8_t*>(&html[oldSize + bytesLoaded])));
		}

		co_return winrt::to_hstring(html);
	}

	std::vector<std::pair<std::string,std::string>> NetworkModule::RegexExtractLinks(std::vector<std::string> const& romList, std::string region, std::string system)
	{
		std::vector<std::pair<std::string, std::string>> extractedLinks;
		auto romsToBeSearched = romList;
		std::sort(romsToBeSearched.begin(), romsToBeSearched.end());

		auto regexPattern = [this, region](std::string const& safeName) {
			return std::format("([A-Z]?)(\\/?)({}.*\\(((World.*)|({}.*))\\).*\\.(7z|zip))", safeName, region);
		};
		for (auto const& romListing : m_romsToDownload)
		{
			if (romListing.romPack != system) continue;
			std::for_each(romListing.romList.begin(), romListing.romList.end(), [this, regexPattern, &romsToBeSearched, &extractedLinks, &romListing](auto const& romData)
			{
				for (auto rom = romsToBeSearched.begin(); rom != romsToBeSearched.end();)
				{
					std::regex romRegex(regexPattern(*rom));
					std::smatch match;
					if (std::regex_search(romData.first, match, romRegex))
					{
						extractedLinks.push_back({ romListing.romPack ,romData.second });
						rom = romsToBeSearched.erase(rom);	
					}
					else ++rom;
				}
				return std::string{};
			});
		}

		return extractedLinks;
	}


}