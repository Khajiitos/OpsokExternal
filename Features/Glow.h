#pragma once

struct GlowStruct {
	float red = 1.f;
	float green = 1.f;
	float blue = 1.f;
	float alpha = 1.f;
	uint8_t padding[8];
	float unknown = 1.f;
	uint8_t padding2[4];
	bool renderOccluded = true;
	bool renderUnoccluded = false;
	bool fullBloom = false;
};

namespace Glow {
	void run();
}