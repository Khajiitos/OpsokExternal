#include <basetsd.h>
#include <cstdint>
#include "Glow.h"
#include "../Offsets.h"
#include "../Globals.h"
#include "../Memory.h"

void Glow::run() {

	if (!config.glow)
		return;

	uintptr_t localPlayer = mem.read<uintptr_t>(clientBase + dwLocalPlayer);
	uintptr_t glowManager = mem.read<uintptr_t>(clientBase + dwGlowObjectManager);

	for (int i = 1; i <= 64; i++) {
		uintptr_t player = mem.read<uintptr_t>(clientBase + dwEntityList + i * 0x10);

		if (!player)
			continue;

		int glowIndex = mem.read<int>(player + m_iGlowIndex);

		if (!glowIndex)
			continue;

		int health = mem.read<int>(player + m_iHealth);
		if (health <= 0 || health > 120)
			continue;

		if (mem.read<bool>(player + m_bDormant))
			continue;

		GlowStruct glowStr;
		glowStr.alpha = config.glowAlpha;

		switch (mem.read<int>(player + m_iTeamNum)) {
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
		mem.write<GlowStruct>(glowManager + (glowIndex * 0x38) + 0x4, glowStr);
	}
}
