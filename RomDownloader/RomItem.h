#pragma once
#include "RomItem.g.h"

#include <winrt/Windows.Storage.h>

using namespace winrt::Windows::Storage;

namespace winrt::RomDownloader::implementation
{
    struct RomItem : RomItemT<RomItem>
    {
        RomItem();
        RomItem(hstring const& name, hstring const& size, hstring const& imageSrc, StorageFile const& file);

        hstring Name() const;
        void Name(hstring const& value);

        hstring Size() const;
        void Size(hstring const& value);

        hstring ImageSrc() const;
        void ImageSrc(hstring const& value);

        StorageFile File() const;
        void File(StorageFile const& value);
    private:
        hstring m_name;
        hstring m_size;
        hstring m_imageSrc;
        StorageFile m_file{ nullptr };
    };
}

namespace winrt::RomDownloader::factory_implementation
{
    struct RomItem : RomItemT<RomItem, implementation::RomItem>
    {
    };
}
