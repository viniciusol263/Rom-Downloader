#pragma once

#include "CloudModule.h"
#include "StorageModule.h"

using namespace winrt::Windows::Foundation;
namespace Controllers
{
	class FrontController
	{
	public:
		FrontController();

		Foundation::IAsyncAction DownloadSystemIcons();
	private:
		Modules::CloudModule m_cloudModule;
		Modules::StorageModule m_storageModule;
	};
}

