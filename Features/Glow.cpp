#include <basetsd.h>
#include <cstdint>
#include "Glow.h"
#include "../Offsets.h"
#include "../Globals.h"
#include "../Memory.h"

GlowStruct terrorist = GlowStruct(0.80f, 0.72f, 0.48f, 1.0f);
GlowStruct counterTerrorist = GlowStruct(0.36f, 0.47f, 0.69f, 1.0f);

void Glow::run() {

	if (!config.glow)
		return;

	uintptr_t localPlayer = mem.read<uintptr_t>(clientBase + dwLocalPlayer);
	uintptr_t glowManager = mem.read<uintptr_t>(clientBase + dwGlowObjectManager);

	if (!localPlayer || !glowManager)
		return;

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

		GlowStruct glowStruct = (mem.read<int>(player + m_iTeamNum) == Team::TERRORISTS ? terrorist : counterTerrorist);
		glowStruct.alpha = config.glowAlpha;

		mem.write<GlowStruct>(glowManager + ((glowIndex * 0x38) + 0x4), glowStruct);
	}
}
