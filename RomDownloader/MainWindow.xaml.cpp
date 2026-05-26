#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include "BlankPage.xaml.h"
#include <winrt/Microsoft.UI.Windowing.h>

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Interop;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Windowing;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::RomDownloader::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
        
        auto appWindow = this->AppWindow();
		appWindow.Title(L"Rom Downloader");
		appWindow.SetIcon(L"Assets/AppIcon.ico");
        appWindow.Resize({ 800,600 });
        //appWindow.Presenter().as<OverlappedPresenter>().IsResizable(false);
        //appWindow.Presenter().as<OverlappedPresenter>().IsMaximizable(false);

        RomDownloader::BlankPage page(*this);

        this->Content(page);
    }

}
