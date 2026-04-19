#pragma once

#include "BlankPage.g.h"
#include "RomItem.g.h"
#include "ButtonController.h"
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace winrt::RomDownloader::implementation
{
    struct BlankPage : BlankPageT<BlankPage>
    {
        BlankPage();

        Controllers::ButtonController m_buttonController;

        void SetPrimaryWindow(winrt::Microsoft::UI::Xaml::Window const& window);
        fire_and_forget DownloadFolderButtonClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        fire_and_forget DownloadClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        fire_and_forget StartRomClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        fire_and_forget ClickedRightTapped(winrt::Windows::Foundation::IInspectable const& ,winrt::Microsoft::UI::Xaml::Input::RightTappedRoutedEventArgs const& e);
        fire_and_forget OpenRomLocationClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        winrt::Windows::Foundation::Collections::IObservableVector<RomDownloader::RomItem> Items();
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> ComboItems();
        winrt::hstring GetRegion();
		winrt::hstring GetSystem();
    private:
        winrt::Microsoft::UI::Xaml::Window m_window{ nullptr };
        winrt::Windows::Foundation::Collections::IObservableVector<RomDownloader::RomItem> m_items{ nullptr };
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> m_comboItems{ nullptr };
    };

    struct RomItem : RomItemT<RomItem>
    {
        RomItem();
        RomItem(hstring const& name, hstring const& size, hstring const& imageSrc);

        hstring Name() const;
        void Name(hstring const& value);

        hstring Size() const;
        void Size(hstring const& value);

		hstring ImageSrc() const;
		void ImageSrc(hstring const& value);
    private:
        hstring m_name;
        hstring m_size;
        hstring m_imageSrc;
    };
}

namespace winrt::RomDownloader::factory_implementation
{
    struct BlankPage : BlankPageT<BlankPage, implementation::BlankPage>
    {
    };

    struct RomItem : RomItemT<RomItem, implementation::RomItem>
    {
	};
}
