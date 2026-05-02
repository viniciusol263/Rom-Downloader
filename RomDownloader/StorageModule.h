#pragma once

#include <string>
#include <filesystem>
#include <memory>

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Foundation.h>

#include <bit7z/bit7zlibrary.hpp>
#include <bit7z/bitextractor.hpp>

using namespace winrt::Windows;
using namespace winrt::Windows::Storage;

namespace Modules
{
	constexpr std::string_view tmpDirectory = "tmp";
	class StorageModule
	{
	public:
		StorageModule();

		StorageFolder GetDownloadPath();
		Foundation::IAsyncOperation<StorageFolder> GetTemporaryPath();

		Foundation::IAsyncAction SetDownloadPath(winrt::hstring const& downloadPath);
		Foundation::IAsyncOperation<Foundation::Collections::IVectorView<StorageFile>> ExtractFilesFromDownloadPath();
		Foundation::IAsyncOperation<Foundation::Collections::IVector<StorageFile>> GetRomsFromDownloadPath();
	private:
		StorageFolder m_downloadPath{ nullptr };
		std::shared_ptr<bit7z::Bit7zLibrary> m_zipLib;
		std::shared_ptr<bit7z::BitExtractor<std::string>> m_zipExtractor;
		std::shared_ptr<bit7z::BitExtractor<std::string>> m_7zExtractor;

	};
}

