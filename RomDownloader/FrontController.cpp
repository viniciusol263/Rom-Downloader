#include "pch.h"
#include "FrontController.h"
#include "Constants.h"

namespace Controllers
{
	FrontController::FrontController()
	{}

	Foundation::IAsyncAction FrontController::DownloadSystemIcons()
	{
		auto folderPath = co_await m_storageModule.CreateFolder(winrt::to_hstring(Constants::SystemIconsPath));
		auto listing = m_cloudModule.GetListing(Constants::SystemIconsPath);
		
		for (const auto& item : listing)
		{
			co_await m_cloudModule.GetObject(item, winrt::to_string(folderPath));
		}
	}
}