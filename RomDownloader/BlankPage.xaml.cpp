#include "pch.h"
#include "BlankPage.xaml.h"
#if __has_include("BlankPage.g.cpp")
#include "BlankPage.g.cpp"
#endif
#if __has_include("RomItem.g.cpp")
#include "RomItem.g.cpp"
#endif

#include "Constants.h"
#include <winrt/Windows.Storage.FileProperties.h>

using namespace winrt::Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::RomDownloader::implementation
{
	RomItem::RomItem() 
    {}

    RomItem::RomItem(hstring const& name, hstring const& size, hstring const& imageSrc)
    : m_name(name), m_size(size), m_imageSrc(imageSrc)
    {}
    

    hstring RomItem::Name() const
    {
		return m_name;
    }
    void RomItem::Name(hstring const& value)
    {
		m_name = value;
    }
    hstring RomItem::Size() const
    {
		return m_size;
    }
    void RomItem::Size(hstring const& value)
    {
		m_size = value;
    }
    hstring RomItem::ImageSrc() const
    {
        return m_imageSrc;
    }
    void RomItem::ImageSrc(hstring const& value)
    {
		m_imageSrc = value;
    }

    BlankPage::BlankPage()
    {
        InitializeComponent();
        m_items = winrt::single_threaded_observable_vector<RomDownloader::RomItem>();
        m_comboItems = winrt::single_threaded_observable_vector<winrt::hstring>({L"All"});
		SystemComboBox().SelectedIndex(0);
		for (auto const& system : Constants::SystemsUrl) 
            m_comboItems.Append(winrt::to_hstring(system.first));
    }

    void BlankPage::SetPrimaryWindow(winrt::Microsoft::UI::Xaml::Window const& window)
    {
        m_window = window;
    }

    fire_and_forget BlankPage::DownloadFolderButtonClick(IInspectable const& sender, RoutedEventArgs const& args)
    {
        if (m_window == nullptr) co_return;
        auto path = co_await m_buttonController.DownloadFolderAction(m_window);
        DownloadTextBox().Text(path);
        co_return;
    }

    fire_and_forget BlankPage::DownloadClick(IInspectable const& sender, RoutedEventArgs const& args)
    {
        auto dispatcher = DispatcherQueue();
        auto op = m_buttonController.DownloadButtonAction(RomsTextBox().Text(), GetRegion(), GetSystem());
        auto found = [this](RomDownloader::RomItem const& item) {
            for (auto rom : m_items)
            {
                if (rom.Name() == item.Name())
                {
                    return true;
                }
            }
            return false;
        };

        op.Progress([this, dispatcher](auto const&, int value)
            {
                if (value == 200)
                {
					ProgressBarDownload().IsIndeterminate(true);
                }
                else {
                    dispatcher.TryEnqueue([this, value]() {
                        ProgressBarDownload().IsIndeterminate(false);
                        ProgressBarDownload().Value(value);
                        });
                }
            });

        auto romList = co_await op;
        auto romListSize = romList.Size();
        for (auto rom : romList)
        {
			auto size = co_await rom.Key().GetBasicPropertiesAsync();
            auto image = Constants::SystemMap.at(winrt::to_string(rom.Value()));
            auto romItem = RomDownloader::RomItem(rom.Key().Name(), winrt::to_hstring(size.Size() / 1000) + L" KB", winrt::to_hstring(image));

            if(!found(romItem))
			    m_items.Append(romItem);
        }
    }

    fire_and_forget BlankPage::ClickedRightTapped(winrt::Windows::Foundation::IInspectable const& , winrt::Microsoft::UI::Xaml::Input::RightTappedRoutedEventArgs const& e)
    {
        auto element = e.OriginalSource().as<Microsoft::UI::Xaml::FrameworkElement>();

        while (element)
        {
            if (auto container = element.try_as<Microsoft::UI::Xaml::Controls::ListViewItem>())
            {
                DownloadRomList().SelectedItem(container.Content());
                break;
            }
            element = element.Parent().try_as<Microsoft::UI::Xaml::FrameworkElement>();
        }
        co_return;
    }

    fire_and_forget BlankPage::StartRomClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args)
    {
		auto romItem = DownloadRomList().SelectedItem().try_as<RomDownloader::RomItem>();

        if (!romItem) co_return;

        auto name = romItem.Name();
        co_return;
    }

    fire_and_forget BlankPage::OpenRomLocationClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args)
    {
        co_return;
    }

    winrt::Windows::Foundation::Collections::IObservableVector<RomDownloader::RomItem> BlankPage::Items()
    {
        return m_items;
    }

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> BlankPage::ComboItems()
    {
		return m_comboItems;
    }

    winrt::hstring BlankPage::GetRegion()
    {
        if (USARegionRadio().IsChecked().Value())
			return L"USA";
		else if (EURRegionRadio().IsChecked().Value())
            return L"EUR";
		else if (JPNRegionRadio().IsChecked().Value())
            return L"Japan";
    }

    winrt::hstring BlankPage::GetSystem()
    {
        if (SystemComboBox().SelectedItem() == nullptr) return L"";
		return SystemComboBox().SelectedItem().as<winrt::hstring>();
    }


}
