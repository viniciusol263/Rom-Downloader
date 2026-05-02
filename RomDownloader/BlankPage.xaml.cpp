#include "pch.h"
#include "BlankPage.xaml.h"
#include "RomItem.h"
#if __has_include("BlankPage.g.cpp")
#include "BlankPage.g.cpp"
#endif

#include "Constants.h"
#include <winrt/Windows.Storage.FileProperties.h>
#include <winrt/Windows.System.h>

using namespace winrt::Windows::System;
using namespace winrt::Microsoft::UI::Xaml;
// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::RomDownloader::implementation
{
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
        if (path.Size() == 0) co_return;
        m_items.Clear();
        DownloadTextBox().Text(path.First().Current().Key());
        co_await InsertRomsOnList(path.First().Current().Value());
        co_return;
    }

    fire_and_forget BlankPage::DownloadClick(IInspectable const& sender, RoutedEventArgs const& args)
    {
        auto dispatcher = DispatcherQueue();
        auto op = m_buttonController.DownloadButtonAction(RomsTextBox().Text(), GetRegion(), GetSystem());
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
        co_await InsertRomsOnList(romList);
    }

#pragma region ListViewEvents
    fire_and_forget BlankPage::StartRomClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args)
    {
		auto menuItem = sender.as<Microsoft::UI::Xaml::Controls::MenuFlyoutItem>();
		auto romItem = menuItem.Tag().try_as<RomDownloader::RomItem>();

        auto file = romItem.File();

		co_await Launcher::LaunchFileAsync(file);
        co_return;
    }

    fire_and_forget BlankPage::OpenRomLocationClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args)
    {
        auto menuItem = sender.as<Microsoft::UI::Xaml::Controls::MenuFlyoutItem>();
        auto romItem = menuItem.Tag().try_as<RomDownloader::RomItem>();

        auto file = romItem.File();

		co_await Launcher::LaunchFolderAsync(co_await file.GetParentAsync());
        co_return;
    }
    
    fire_and_forget BlankPage::OnItemDoubleTapped(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::DoubleTappedRoutedEventArgs const& args)
    {
        auto romItem = DownloadRomList().SelectedItem().as<RomDownloader::RomItem>();
        auto file = romItem.File();
        co_await Launcher::LaunchFileAsync(file);
        co_return;
    }

    fire_and_forget BlankPage::OnListKeyDown(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& args)
    {
        if (args.Key() == winrt::Windows::System::VirtualKey::Enter ||
            args.Key() == winrt::Windows::System::VirtualKey::Space)
        {
            auto romItem = DownloadRomList().SelectedItem().as<RomDownloader::RomItem>();
            if (romItem)
            {
                auto file = romItem.File();
                co_await Launcher::LaunchFileAsync(file);
            }
        }
        co_return;
    }
#pragma endregion

#pragma region IDLProperties
    winrt::Windows::Foundation::Collections::IObservableVector<RomDownloader::RomItem> BlankPage::Items()
    {
        return m_items;
    }

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> BlankPage::ComboItems()
    {
		return m_comboItems;
    }
#pragma endregion

#pragma region Getters
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
#pragma endregion

#pragma region PrivateHelpers
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IObservableVector<RomDownloader::RomItem>> 
        BlankPage::InsertRomsOnList(winrt::Windows::Foundation::Collections::IMap<StorageFile, winrt::hstring> romList)
    {
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
        for (auto rom : romList)
        {
            auto size = co_await rom.Key().GetBasicPropertiesAsync();
            auto image = Constants::SystemMap.at(winrt::to_string(rom.Value()));
            auto romItem = RomDownloader::RomItem(rom.Key().Name(), winrt::to_hstring(size.Size() / 1000) + L" KB", winrt::to_hstring(image), rom.Key());

            if (!found(romItem))
                m_items.Append(romItem);
        }
		co_return m_items;
    }
#pragma endregion
}
