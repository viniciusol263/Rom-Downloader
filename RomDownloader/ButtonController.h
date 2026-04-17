#pragma once

#include "NetworkModule.h"
#include "StorageModule.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>

using namespace winrt::Windows;

namespace Controllers
{
	class ButtonController
	{
	public:
		ButtonController();

		Foundation::IAsyncOperation<winrt::hstring> DownloadFolderAction(winrt::Microsoft::UI::Xaml::Window const& window);
		Foundation::IAsyncOperationWithProgress<Foundation::Collections::IMap<StorageFile, winrt::hstring>, int> DownloadButtonAction(winrt::hstring roms, winrt::hstring region);
	private:
		Modules::NetworkModule m_networkModule;
		Modules::StorageModule m_storageModule;

	};
}


