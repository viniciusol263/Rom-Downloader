#include "pch.h"
#include "ParserHandler.h"

namespace Handlers
{
	ParserHandler::ParserHandler()
	{}

	void ParserHandler::ParseDOMContent(std::string const& content)
	{
		m_pageOutput = std::shared_ptr<GumboOutput>(gumbo_parse(content.c_str()));
		GatherLinks(m_pageOutput->root);
		auto elem = m_links; //DEBUG ONLY
	}

	std::vector<std::pair<std::string, std::string>> ParserHandler::GetLinks()
	{
		auto ret = m_links;
		m_links.clear();
		return ret;
	}

	std::vector<std::string> ParserHandler::GetRomList(std::string_view romsToDownload)
	{
		auto index = 0;
		auto delimiter = ";";
		
		if (romsToDownload.find(delimiter) == std::string_view::npos)
		{
			delimiter = ",";
		}
		auto end = romsToDownload.find(delimiter);
		std::vector<std::string> romList;
		if (end != std::string_view::npos)
		{
			while (end != std::string_view::npos)
			{
				// Process each rom
				romList.push_back(std::string(romsToDownload.substr(index, end - index)));
				index = end + 1;
				end = romsToDownload.find(delimiter, index);
			}
		}
		romList.push_back(std::string(romsToDownload.substr(index, end - index)));
		return romList;
	}

	void ParserHandler::GatherLinks(GumboNode* node)
	{
		if (!node || node->type != GUMBO_NODE_ELEMENT ) return;
		if (node->v.element.tag == GUMBO_TAG_A)
		{
			std::pair<std::string, std::string> result = {};
			GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
			if (href)
			{
				std::string link = href->value;
				if (link.starts_with("//")) link = "https:" + link;
				result.second = link;
			}
			const GumboVector* textNode = &node->v.element.children;
			if (textNode != nullptr)
			{
				auto textNodeLink = static_cast<GumboNode*>(textNode->data[0]);
				if (textNodeLink->type == GUMBO_NODE_TEXT)
				{
					result.first = textNodeLink->v.text.text;
				}
			}
			m_links.push_back(result);
		}
		const GumboVector* children = &node->v.element.children;
		for (auto index = 0; children && index < children->length; ++index)
		{
			auto nodeLink = static_cast<GumboNode*>(children->data[index]);
			GatherLinks(nodeLink);
		}

	}

}