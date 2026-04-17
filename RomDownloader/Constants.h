#pragma once

#include <utility>
#include <string>
#include <vector>
#include <map>

namespace Constants
{
	const std::vector<std::pair<std::string, std::string>> SystemsUrl = {
		{"Game Boy", "https://ia801404.us.archive.org/view_archive.php?archive=/7/items/no-intro_romset_collection/Nintendo%20-%20Game%20Boy.7z"},
		{"Game Boy Color", "https://ia801404.us.archive.org/view_archive.php?archive=/7/items/no-intro_romset_collection/Nintendo%20-%20Game%20Boy%20Color.7z"},
		{"WonderSwan", "https://ia801404.us.archive.org/view_archive.php?archive=/7/items/no-intro_romset_collection/Bandai%20-%20WonderSwan.7z"},
		{"WonderSwan Color", "https://ia601404.us.archive.org/view_archive.php?archive=/7/items/no-intro_romset_collection/Bandai%20-%20WonderSwan%20Color.7z"}
	};
	
	const std::map<std::string, std::string> SystemMap = {
		{"Game Boy", "ms-appx:///Assets/Nintendo_Game_Boy_Logo.svg"},
		{"Game Boy Color","ms-appx:///Assets/Game_Boy_Color_logo.svg"},
		{"WonderSwan", "ms-appx:///Assets/WonderSwan_logo.svg"},
		{"WonderSwan Color", "ms-appx:///Assets/WonderSwan_logo.svg"}
	};
}