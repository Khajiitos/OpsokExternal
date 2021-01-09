#include <basetsd.h>
#include <cstdint>
#include "RadarHack.h"
#include "../Offsets.h"
#include "../Globals.h"
#include "../Memory.h"
#include "../Vector.h"

void RadarHack::run() {

	if (!config.radarHack)
		return;

	uintptr_t localPlayer = mem.read<uintptr_t>(clientBase + dwLocalPlayer);

	if (!localPlayer)
		return;

	for (int i = 1; i <= 64; i++) {

		uintptr_t player = mem.read<uintptr_t>(clientBase + dwEntityList + i * 0x10);

		if (!player)
			continue;

		if (mem.read<bool>(player + m_bDormant))
			continue;

		if (player == localPlayer)
			continue;

		mem.write<bool>(player + m_bSpotted, true);
	}
}
