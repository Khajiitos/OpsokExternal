#include "UserInput.h"
#include <iostream>
#include <string>
#include <array>
#include <cstring>
#include "Offsets.h"
#include "Globals.h"

struct PlayerInfo {
	byte pad[8];
	union {
		__int64 steamID64;
		struct {
			int txuid_low;
			int xuid_high;
		};
	};
	char name[128];
	int userId;
	char szSteamID[20];
	byte pad1[16];
	unsigned long iSteamID;
	char szFriendsName[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};

#define print(text) std::cout << text << std::endl

void UserInput::loop() {
	while (true) {
		std::cout << "> ";
		std::string cmd;
		std::getline(std::cin, cmd);

		if (cmd == "help") printCommands();
		else if (cmd == "printranks") printRanks();
		else print("Unknown command. Type \"help\" for a list of commands.");
	}
}

void UserInput::printCommands() {
	print("Available commands:");
	print(" printranks - prints ranks and wins of all players on the server (doesn't work in non-competitive modes)");
}

void UserInput::printRanks() {

	// BROKEN

	const char* ranks[] = {
		"Unranked",

		"Silver I",
		"Silver II",
		"Silver III",
		"Silver IV",
		"Silver Elite",
		"Silver Elite Master",

		"Gold Nova I",
		"Gold Nova II",
		"Gold Nova III",
		"Gold Nova Master",

		"Master Guardian I",
		"Master Guardian II",
		"Master Guardian Elite",
		"Distinguished Master Guardian",

		"Legendary Eagle",
		"Legendary Eagle Master",

		"Supreme Master First Class",
		"The Global Elite"
	};

	uintptr_t playerResource = mem.read<uintptr_t>(clientBase + dwPlayerResource);
	uintptr_t clientState = mem.read<uintptr_t>(engineBase + dwClientState);
	uintptr_t userInfoTable = mem.read<uintptr_t>(clientState + dwClientState_PlayerInfo);
	uintptr_t items = mem.read<uintptr_t>(mem.read<uintptr_t>(userInfoTable + 0x40) + 0xC);

	for (int i = 1; i <= 64; i++) {
		uintptr_t player = mem.read<uintptr_t>(clientBase + dwEntityList + i * 0x10);

		if (!player)
			continue;

		const char* rank = ranks[mem.read<int>(playerResource + m_iCompetitiveRanking + (i * 4))];
		int wins = mem.read<int>(playerResource + m_iCompetitiveWins + (i * 4));

		PlayerInfo playerInfo = mem.read<PlayerInfo>(mem.read<uintptr_t>(items + 0x28 + ((i) * 0x34)));

		if (playerInfo.ishltv || playerInfo.fakeplayer)
			continue;

		std::string str = playerInfo.name;
		str += " - ";
		str += rank;
		str += ", ";
		str += std::to_string(wins);
		str += " wins";

		print(str);
	}
}