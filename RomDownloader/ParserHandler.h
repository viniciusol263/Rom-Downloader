#pragma once

#include <gumbo.h>
#include <memory>
#include <string>
#include <vector>
#include <utility>

namespace Handlers
{
	class ParserHandler
	{
	public:
		ParserHandler();

		void ParseDOMContent(std::string const& content);
		std::vector<std::pair<std::string, std::string>> GetLinks();
		std::vector<std::string> GetRomList(std::string_view romsToDownload);

	private:
		std::shared_ptr<GumboOutput> m_pageOutput;
		std::vector<std::pair<std::string, std::string>> m_links;

		void GatherLinks(GumboNode* node);
	};
}

