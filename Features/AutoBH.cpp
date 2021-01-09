#include "AutoBH.h"
#include "../Offsets.h"
#include "../Globals.h"

void AutoBH::run() {

	if (!config.autoBh)
		return;

	uintptr_t localPlayer = mem.read<uintptr_t>(clientBase + dwLocalPlayer);

	if (!localPlayer)
		return;

	bool onGround = mem.read<DWORD>(localPlayer + m_fFlags) & Flags::ON_GROUND;

	if (GetAsyncKeyState(VK_SPACE) && onGround)
		mem.write<DWORD>(clientBase + dwForceJump, 6);
}
