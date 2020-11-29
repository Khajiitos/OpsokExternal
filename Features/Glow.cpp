#include <basetsd.h>
#include <cstdint>
#include "Glow.h"
#include "../Offsets.h"
#include "../Globals.h"
#include "../Memory.h"

void Glow::run() {

	if (!config.glow)
		return;

	uintptr_t localPlayer = mem.read<uintptr_t>(moduleBase + dwLocalPlayer);
	uintptr_t dwGlowManager = mem.read<uintptr_t>(moduleBase + dwGlowObjectManager);
	int LocalTeam = mem.read<int>(localPlayer + m_iTeamNum);
	for (int i = 1; i < 64; i++) {
		uintptr_t dwEntity = mem.read<uintptr_t>(moduleBase + dwEntityList + i * 0x10);

		if (!dwEntity)
			continue;

		int iGlowIndx = mem.read<int>(dwEntity + m_iGlowIndex);
		int entHealth = mem.read<int>(dwEntity + m_iHealth);

		if (entHealth <= 0)
			continue;

		bool dormant = mem.read<bool>(dwEntity + m_bDormant);

		if (dormant)
			continue;

		int entTeam = mem.read<int>(dwEntity + m_iTeamNum);

		GlowStruct glowStr;
		glowStr.alpha = 0.75f;

		switch (entTeam) {
		case Team::SPECTATORS:
			glowStr.red = 1.0f;
			glowStr.green = 1.0f;
			glowStr.blue = 1.0f;
			break;
		case Team::TERRORISTS:
			glowStr.red = 0.80f;
			glowStr.green = 0.72f;
			glowStr.blue = 0.48f;
			break;
		case Team::COUNTERTERRORISTS:
			glowStr.red = 0.36f;
			glowStr.green = 0.47f;
			glowStr.blue = 0.69f;
			break;
		}

		mem.write<GlowStruct>(dwGlowManager + (iGlowIndx * 0x38) + 0x4, glowStr);

	}
}
