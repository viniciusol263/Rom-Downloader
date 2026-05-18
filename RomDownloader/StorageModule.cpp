#include "pch.h"
#include "StorageModule.h"
#include "Constants.h"

namespace Modules
{
	StorageModule::StorageModule()
	{
		auto exePath = baseDirectory + winrt::to_hstring("\\..\\7z.dll");
		m_zipLib = std::make_shared<bit7z::Bit7zLibrary>(winrt::to_string(exePath));
		m_zipExtractor = std::make_shared<bit7z::BitExtractor<std::string>>(*m_zipLib, bit7z::BitFormat::Zip);
		m_7zExtractor = std::make_shared<bit7z::BitExtractor<std::string>>(*m_zipLib, bit7z::BitFormat::SevenZip);
	}

	StorageFolder StorageModule::GetDownloadPath()
	{
		return m_downloadPath;
	}

	Foundation::IAsyncOperation<StorageFolder> StorageModule::GetTemporaryPath()
	{
		co_await m_downloadPath.CreateFolderAsync(winrt::to_hstring(tmpDirectory), CreationCollisionOption::OpenIfExists);
		co_return co_await m_downloadPath.GetFolderAsync(winrt::to_hstring(tmpDirectory));
	}

	Foundation::IAsyncOperation<winrt::hstring> StorageModule::CreateFolder(winrt::hstring const& folderName)
	{
		auto folder = co_await StorageFolder::GetFolderFromPathAsync(baseDirectory);
		auto path = co_await folder.CreateFolderAsync(folderName, CreationCollisionOption::OpenIfExists);
		co_return path.Path();
	}

	Foundation::IAsyncAction StorageModule::SetDownloadPath(winrt::hstring const& downloadPath)
	{
		m_downloadPath = co_await StorageFolder::GetFolderFromPathAsync(downloadPath);
	}

	Foundation::IAsyncOperation<Foundation::Collections::IVectorView<StorageFile>> StorageModule::ExtractFilesFromDownloadPath()
	{
		if (m_downloadPath == nullptr) co_return Foundation::Collections::IVectorView<StorageFile>();
		StorageFolder downloadPath = co_await GetTemporaryPath();
		Foundation::Collections::IVectorView<StorageFile> extractedFiles;
		auto isZip = [](StorageFile fileName) {
			return fileName.FileType() == L".zip" || fileName.FileType() == L".7z";
		};

		auto zippedFiles = co_await downloadPath.GetFilesAsync();

		for (auto file : zippedFiles)
		{
			if (!isZip(file)) continue;
			if (file.FileType() == L".zip")
				m_zipExtractor->extract(winrt::to_string(file.Path().c_str()), winrt::to_string(m_downloadPath.Path().c_str()));
			else if (file.FileType() == L".7z")
				m_7zExtractor->extract(winrt::to_string(file.Path().c_str()), winrt::to_string(m_downloadPath.Path().c_str()));
		}

		co_await downloadPath.DeleteAsync();

		extractedFiles = co_await m_downloadPath.GetFilesAsync();

		co_return extractedFiles;
	}

	Foundation::IAsyncOperation<Foundation::Collections::IVector<StorageFile>> StorageModule::GetRomsFromDownloadPath()
	{
		if (m_downloadPath == nullptr) co_return Foundation::Collections::IVector<StorageFile>();

		auto retVector = winrt::single_threaded_vector<StorageFile>();

		Foundation::Collections::IVectorView<StorageFile> files = co_await m_downloadPath.GetFilesAsync();
		
		for (auto file : files)
		{
			if (Constants::FileTypeMap.find(winrt::to_string(file.FileType().c_str())) != Constants::FileTypeMap.end())
			{
				retVector.Append(file);
			}
		}

		co_return retVector;
	}


}