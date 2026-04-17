#include "pch.h"
#include "WindowHandler.h"

#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Windowing.h>


namespace Handlers
{
	HWND WindowHandler::GetWindowHandle(winrt::Microsoft::UI::Xaml::Window const& window)
	{
		HWND hwnd{ nullptr };

		auto windowNative = window.as<IWindowNative>();
		windowNative->get_WindowHandle(&hwnd);

		return hwnd;
	}

}