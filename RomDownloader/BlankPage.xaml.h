#pragma once

#include "BlankPage.g.h"
#include "RomItem.h"
#include "ButtonController.h"
#include "FrontController.h"
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace winrt::RomDownloader::implementation
{
    struct BlankPage : BlankPageT<BlankPage>
    {
        BlankPage();

        Controllers::ButtonController m_buttonController;
		Controllers::FrontController m_frontController;

        void SetPrimaryWindow(winrt::Microsoft::UI::Xaml::Window const& window);
        fire_and_forget DownloadFolderButtonClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        fire_and_forget DownloadClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        fire_and_forget StartRomClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        fire_and_forget OpenRomLocationClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        fire_and_forget OnItemDoubleTapped(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::DoubleTappedRoutedEventArgs const& args);
        fire_and_forget OnListKeyDown(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& args);
        winrt::Windows::Foundation::Collections::IObservableVector<RomDownloader::RomItem> Items();
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> ComboItems();
        winrt::hstring GetRegion();
		winrt::hstring GetSystem();
    private:
        winrt::Microsoft::UI::Xaml::Window m_window{ nullptr };
        winrt::Windows::Foundation::Collections::IObservableVector<RomDownloader::RomItem> m_items{ nullptr };
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> m_comboItems{ nullptr };

        winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IObservableVector<RomDownloader::RomItem>>
            InsertRomsOnList(winrt::Windows::Foundation::Collections::IMap<StorageFile, winrt::hstring> romList);
    };
}

namespace winrt::RomDownloader::factory_implementation
{
    struct BlankPage : BlankPageT<BlankPage, implementation::BlankPage>
    {
    };
}
