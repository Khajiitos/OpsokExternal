#include "Aimbot.h"
#include "../Vector.h"
#include <math.h>
#include "../Offsets.h"
#include "../Globals.h"

#define PI 3.14159265358979323846

constexpr float pi180 = 180.f / (float)PI;

Vector getBonePosition(int entityId, int boneId)
{
	uintptr_t player = mem.read<uintptr_t>(clientBase + dwEntityList + entityId * 0x10);
	uintptr_t boneMatrix = mem.read<uintptr_t>(player + m_dwBoneMatrix);
	Vector bonePos;
	bonePos.x = mem.read<float>(boneMatrix + 0x30 * boneId + 0x0C);
	bonePos.y = mem.read<float>(boneMatrix + 0x30 * boneId + 0x1C);
	bonePos.z = mem.read<float>(boneMatrix + 0x30 * boneId + 0x2C);
	return bonePos;
}

Vector Aimbot::CalcAngle(Vector src, Vector dst)
{
	Vector angles;

	Vector delta = src - dst;
	float hyp = delta.magnitude();
	angles.x = atanf(delta.z / hyp) * pi180;
	angles.y = atanf(delta.y / delta.x) * pi180;
	angles.z = 0.0f;

	if (delta.x >= 0.0f)
		angles.y += 180.0f;

	return angles;
}

bool isSpotted(uintptr_t entity) {
	uintptr_t clientState = mem.read<uintptr_t>(clientBase + dwClientState);
	int localPlayer = mem.read<int>(clientState + dwClientState_GetLocalPlayer);
	return mem.read<int>(entity + m_bSpottedByMask) & (localPlayer << 0);
}

void Aimbot::run() {

	if (!config.aimbot)
		return;

	static bool toggled = true;

	if (GetAsyncKeyState(config.aimbotToggleKey) & 1)
		toggled = !toggled;

	if (!toggled)
		return;

	uintptr_t localPlayer = mem.read<uintptr_t>(clientBase + dwLocalPlayer);
	uintptr_t clientState = mem.read<uintptr_t>(engineBase + dwClientState);

	Vector bestTarget;
	float bestTargetFov = 420.f;

	if (!localPlayer || mem.read<bool>(localPlayer + m_bInReload))
		return;

	if (mem.read<bool>(localPlayer + m_lifeState) != 0)
		return;

	if (!(mem.read<int>(localPlayer + m_fFlags) & Flags::ON_GROUND))
		return;

	for (int i = 1; i <= 64; i++) {

		uintptr_t player = mem.read<uintptr_t>(clientBase + dwEntityList + i * 0x10);

		if (!player)
			continue;

		if (mem.read<bool>(player + m_bDormant))
			continue;

		if (mem.read<bool>(player + m_lifeState) != 0)
			continue;

		if (mem.read<int>(player + m_iTeamNum) == mem.read<int>(localPlayer + m_iTeamNum))
			continue;

		if (mem.read<bool>(player + m_bGunGameImmunity))
			continue;

		if (config.aimbotVisibilityCheck && !isSpotted(player))
			continue;

		if (player == localPlayer)
			continue;

		Vector curViewAngles = mem.read<Vector>(clientState + dwClientState_ViewAngles);

		Vector localplayerpos = mem.read<Vector>(localPlayer + m_vecOrigin) + mem.read<Vector>(localPlayer + m_vecViewOffset);
		Vector enemyplayerpos = getBonePosition(i, 8);

		if (enemyplayerpos.zero())
			continue;

		Vector angle = CalcAngle(localplayerpos, enemyplayerpos);
		Vector relative = curViewAngles - angle;
		relative.normalize();

		const auto fov = std::hypot(relative.x, relative.y);

		if (fov > config.aimbotFov)
			continue;

		if (bestTargetFov > fov) {
			bestTargetFov = fov;
			angle.normalize();
			bestTarget = angle;
		}

	}

	if (!bestTarget.zero()) {

		if (config.aimbotControlRecoil)
			bestTarget -= (mem.read<Vector>(localPlayer + m_aimPunchAngle)) * 2;

		bestTarget.normalize();
		mem.write<Vector>(clientState + dwClientState_ViewAngles, bestTarget);
	}
}
