#pragma once

#include "BlankPage.g.h"
#include "RomItem.g.h"
#include "ButtonController.h"

namespace winrt::RomDownloader::implementation
{
    struct BlankPage : BlankPageT<BlankPage>
    {
        BlankPage();

        Controllers::ButtonController m_buttonController;

        void SetPrimaryWindow(winrt::Microsoft::UI::Xaml::Window const& window);
        fire_and_forget DownloadFolderButtonClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        fire_and_forget DownloadClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);
        winrt::Windows::Foundation::Collections::IObservableVector<RomDownloader::RomItem> Items();
        winrt::hstring GetRegion();
    private:
        winrt::Microsoft::UI::Xaml::Window m_window{ nullptr };
        winrt::Windows::Foundation::Collections::IObservableVector<RomDownloader::RomItem> m_items{ nullptr };

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
