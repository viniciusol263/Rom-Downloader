#include "pch.h"
#include <print>
#include "ButtonController.h"
#include <chrono>
#include "UIHandler.h"
#include "Constants.h"

namespace Controllers
{
	ButtonController::ButtonController()
	{}

	Foundation::IAsyncOperation<Foundation::Collections::IMap<winrt::hstring, Foundation::Collections::IMap<StorageFile, winrt::hstring>>>
		ButtonController::DownloadFolderAction(winrt::Microsoft::UI::Xaml::Window const& window)
	{
		auto retMap = winrt::single_threaded_map<winrt::hstring, Foundation::Collections::IMap<StorageFile, winrt::hstring>>();
		auto folder = co_await Handlers::UIHandler::ScheduleFolderPicker(window);
		if (folder == nullptr) co_return retMap;
		co_await m_storageModule.SetDownloadPath(folder.Path());
		auto files = co_await m_storageModule.GetRomsFromDownloadPath();
		auto romList = co_await CreateRomList(files);
		retMap.Insert(folder.Path(), romList);

		co_return retMap;
	}

	Foundation::IAsyncOperationWithProgress<Foundation::Collections::IMap<StorageFile,winrt::hstring>,int> 
		ButtonController::DownloadButtonAction(winrt::hstring roms, winrt::hstring region, winrt::hstring system)
	{
		if (roms == L"" || system == L"" || m_storageModule.GetDownloadPath() == nullptr) co_return winrt::single_threaded_map<StorageFile, winrt::hstring>();
		auto returnMap = winrt::single_threaded_map<StorageFile, winrt::hstring>();

		auto progress = co_await winrt::get_progress_token();

		auto romSystems = co_await m_networkModule.DownloadRoms(roms, region, system, co_await m_storageModule.GetTemporaryPath(), [progress](int value) {
			progress(value);
			});

		auto extractedFiles = co_await m_storageModule.ExtractFilesFromDownloadPath();

		auto mapSize = romSystems.Size();
		for(auto const& file : extractedFiles)
		{
			for (auto const& romSystem : romSystems)
			{
				auto value = winrt::to_string(romSystem.Value());
				auto key = winrt::to_string(romSystem.Key());
				auto fileName = winrt::to_string(file.Name());
				fileName.erase(fileName.find(winrt::to_string(file.FileType())));
					
				if (key.find(fileName) != std::string::npos)
				{
					returnMap.Insert(file,romSystem.Value());
				}
			}
		}

		co_return returnMap;
	}

	Foundation::IAsyncOperation<Foundation::Collections::IMap<StorageFile, winrt::hstring>>
		ButtonController::CreateRomList(Foundation::Collections::IVector<StorageFile> const& roms)
	{
		auto romList = winrt::single_threaded_map<StorageFile, winrt::hstring>();
		for (auto const& rom : roms)
		{
			romList.Insert(rom, winrt::to_hstring(Constants::FileTypeMap.at(winrt::to_string(rom.FileType()))));
		}

		co_return romList;
	}
}