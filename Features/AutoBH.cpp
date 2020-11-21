#include "AutoBH.h"
#include "../Offsets.h"
#include "../Globals.h"
#include "../Vector.h"

void AutoBH::run() {

	if (!config.autoBh)
		return;

	uintptr_t localPlayer = mem.read<uintptr_t>(moduleBase + dwLocalPlayer);
	Vector velocity = mem.read<Vector>(localPlayer + m_vecVelocity);
	bool onGround = mem.read<DWORD>(localPlayer + m_fFlags) & Flags::ON_GROUND;

	if (GetAsyncKeyState(VK_SPACE)) {
		if (onGround) {
			mem.write<DWORD>(moduleBase + dwForceJump, 6);
		}
	}

}