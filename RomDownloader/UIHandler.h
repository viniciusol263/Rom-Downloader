#pragma once

#include "WindowHandler.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>

using namespace winrt::Windows;
using namespace winrt::Windows::Storage;

namespace Handlers
{
	class UIHandler
	{
	public:
		static Foundation::IAsyncOperation<StorageFolder> ScheduleFolderPicker(winrt::Microsoft::UI::Xaml::Window const& window);
	};
}

