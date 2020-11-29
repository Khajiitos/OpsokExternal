#include "Triggerbot.h"
#include "../Globals.h"
#include "../Offsets.h"

void Triggerbot::run() {

	if (!config.triggerbot)
		return;

	if (!GetAsyncKeyState(config.triggerbotKey))
		return;

	uintptr_t localPlayer = mem.read<uintptr_t>(moduleBase + dwLocalPlayer);
	int crosshairPlayer = mem.read<int>(localPlayer + m_iCrosshairId);

	if (crosshairPlayer != 0 && crosshairPlayer < 64) {
		uintptr_t player = mem.read<uintptr_t>(moduleBase + dwEntityList + (crosshairPlayer - 1) * 0x10);
		int playerTeam = mem.read<int>(player + m_iTeamNum);
		int localPlayerTeam = mem.read<int>(localPlayer + m_iTeamNum);

		if (playerTeam == localPlayerTeam)
			return;

		if (mem.read<bool>(player + m_bDormant))
			return;

		if (mem.read<bool>(player + m_bGunGameImmunity))
			return;

		mem.write<DWORD>(moduleBase + dwForceAttack, 6);
	}
}
