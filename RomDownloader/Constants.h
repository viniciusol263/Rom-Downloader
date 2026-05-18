#pragma once

#include <utility>
#include <string>
#include <vector>
#include <map>

namespace Constants
{
	const std::string SystemIconsPath = "systemIcons";
	const std::string AccessKey = "AKIAZBDC3RXC3W3FS22G";
	const std::string SecretKey = "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";

	const std::vector<std::pair<std::string, std::string>> SystemsUrl = {
		{"Game Boy", "https://ia801404.us.archive.org/view_archive.php?archive=/7/items/no-intro_romset_collection/Nintendo%20-%20Game%20Boy.7z"},
		{"Game Boy Color", "https://ia801404.us.archive.org/view_archive.php?archive=/7/items/no-intro_romset_collection/Nintendo%20-%20Game%20Boy%20Color.7z"},
		{"Game Boy Advance", "https://archive.org/download/no-intro_romset_collection/Nintendo%20-%20Game%20Boy%20Advance.7z/"},
		{"WonderSwan", "https://ia801404.us.archive.org/view_archive.php?archive=/7/items/no-intro_romset_collection/Bandai%20-%20WonderSwan.7z"},
		{"WonderSwan Color", "https://ia601404.us.archive.org/view_archive.php?archive=/7/items/no-intro_romset_collection/Bandai%20-%20WonderSwan%20Color.7z"}
	};
	
	const std::map<std::string, std::string> SystemMap = {
		{"Game Boy", "ms-appx:///" + SystemIconsPath + "/Nintendo_Game_Boy_Logo.svg"},
		{"Game Boy Color","ms-appx:///" + SystemIconsPath + "/Game_Boy_Color_logo.svg"},
		{"Game Boy Advance", "ms-appx:///" + SystemIconsPath + "/Game_Boy_Advance_logo.svg"},
		{"WonderSwan", "ms-appx:///" + SystemIconsPath + "/WonderSwan_logo.svg"},
		{"WonderSwan Color", "ms-appx:///" + SystemIconsPath + "/WonderSwan_logo.svg"}
	};

	const std::map<std::string, std::string> FileTypeMap = {
		{".gb", "Game Boy" },
		{".gbc", "Game Boy Color"},
		{".gba", "Game Boy Advance"},
		{".ws", "WonderSwan"},
		{".wsc", "WonderSwan Color"}
	};

}