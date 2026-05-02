#include "pch.h"
#include "RomItem.h"
#if __has_include("RomItem.g.cpp")
#include "RomItem.g.cpp"
#endif

namespace winrt::RomDownloader::implementation
{
	RomItem::RomItem() 
	{}

	RomItem::RomItem(hstring const& name, hstring const& size, hstring const& imageSrc, StorageFile const& file)
	: m_name(name), m_size(size), m_imageSrc(imageSrc), m_file(file)
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

	StorageFile RomItem::File() const
	{
		return m_file;
	}
	void RomItem::File(StorageFile const& value)
	{
		m_file = value;
	}
}