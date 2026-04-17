#include "pch.h"
#include <Shobjidl.h>
#include "UIHandler.h"

#include <winrt/Windows.Storage.Pickers.h>

using namespace winrt::Windows::Storage::Pickers;

namespace Handlers
{
	Foundation::IAsyncOperation<StorageFolder> UIHandler::ScheduleFolderPicker(winrt::Microsoft::UI::Xaml::Window const& window)
	{
		FolderPicker picker;
		picker.SuggestedStartLocation(PickerLocationId::Downloads);
		picker.FileTypeFilter().Append(L"*");

		auto hwnd = WindowHandler::GetWindowHandle(window);
		picker.as<::IInitializeWithWindow>()->Initialize(hwnd);

		auto folder = co_await picker.PickSingleFolderAsync();

		co_return folder;
	}
}