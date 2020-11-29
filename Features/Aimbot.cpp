#include "Aimbot.h"
#include "../Vector.h"
#include <math.h>
#include "../Offsets.h"
#include "../Globals.h"

#define PI 3.14159265358979323846

constexpr float pi180 = 180.f / (float)PI;

Vector getBonePosition(int entityId, int boneId)
{
	uintptr_t player = mem.read<uintptr_t>(moduleBase + dwEntityList + entityId * 0x10);
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

void Aimbot::run() {

	if (!config.aimbot)
		return;

	static bool toggled = true;

	if (GetAsyncKeyState(config.aimbotToggleKey) & 1)
		toggled = !toggled;

	if (!toggled)
		return;

	uintptr_t localPlayer = mem.read<uintptr_t>(moduleBase + dwLocalPlayer);
	uintptr_t clientState = mem.read<uintptr_t>(engineModule + dwClientState);

	Vector bestTarget;
	float bestTargetFov = 255.f;
	float maxfov = 255.f;

	if (!localPlayer || mem.read<bool>(localPlayer + m_bInReload))
		return;

	for (int i = 1; i < 64; i++) {

		uintptr_t player = mem.read<uintptr_t>(moduleBase + dwEntityList + i * 0x10);

		if (!player)
			continue;

		if (mem.read<bool>(player + m_bDormant))
			continue;

		if (mem.read<bool>(player + m_lifeState) != 0)
			continue;

		if (mem.read<int>(player + m_iTeamNum) == mem.read<int>(localPlayer + m_iTeamNum))
			continue;

		if (player == localPlayer)
			continue;

		Vector curViewAngles = mem.read<Vector>(clientState + dwClientState_ViewAngles);


		Vector localplayerpos = mem.read<Vector>(localPlayer + m_vecOrigin) + mem.read<Vector>(localPlayer + m_vecViewOffset);
		Vector enemyplayerpos = getBonePosition(i, 8);

		Vector angle = CalcAngle(localplayerpos, enemyplayerpos);
		Vector relative = curViewAngles - angle;
		relative.normalize();

		const auto fov = std::hypot(relative.x, relative.y);

		if (fov > maxfov)
			continue;

		if (bestTargetFov > fov) {
			bestTargetFov = fov;
			angle.normalize();
			bestTarget = angle;
		}

	}

	if (!bestTarget.zero())
		mem.write<Vector>(clientState + dwClientState_ViewAngles, bestTarget);

}
