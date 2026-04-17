#pragma once

#include <Windows.h>

#include <winrt/base.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.UI.Xaml.h>

#include <microsoft.ui.xaml.window.h>

namespace Handlers
{
	class WindowHandler
	{
	public:
		static HWND GetWindowHandle(winrt::Microsoft::UI::Xaml::Window const& window);
	};
}

