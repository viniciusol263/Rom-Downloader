#pragma once

#include "MainWindow.g.h"
#include "BlankPage.xaml.h"
#include <winrt/Windows.UI.Xaml.Interop.h>

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;

namespace winrt::RomDownloader::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();
    };
}

namespace winrt::RomDownloader::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
