#pragma once

#include "NetworkModule.h"
#include "StorageModule.h"
#include "CloudModule.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>

using namespace winrt::Windows;

namespace Controllers
{
	class ButtonController
	{
	public:
		ButtonController();

		Foundation::IAsyncOperation<Foundation::Collections::IMap<winrt::hstring, Foundation::Collections::IMap<StorageFile, winrt::hstring>>>
			DownloadFolderAction(winrt::Microsoft::UI::Xaml::Window const& window);
		Foundation::IAsyncOperationWithProgress<Foundation::Collections::IMap<StorageFile, winrt::hstring>, int> 
			DownloadButtonAction(winrt::hstring roms, winrt::hstring region, winrt::hstring system);
	private:
		Modules::NetworkModule m_networkModule;
		Modules::StorageModule m_storageModule;
		Modules::CloudModule m_cloudModule;

		Foundation::IAsyncOperation<Foundation::Collections::IMap<StorageFile, winrt::hstring>>
			CreateRomList(Foundation::Collections::IVector<StorageFile> const& roms);
	};
}


