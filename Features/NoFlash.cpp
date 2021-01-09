#include "NoFlash.h"
#include "../Globals.h"
#include "../Offsets.h"

void NoFlash::run() {

	if (!config.noFlash)
		return;

	uintptr_t localPlayer = mem.read<uintptr_t>(clientBase + dwLocalPlayer);

	if (!localPlayer)
		return;

	mem.write<float>(localPlayer + m_flFlashDuration, 0.f);
}