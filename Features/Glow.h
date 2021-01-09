#pragma once

struct GlowStruct {
	float red = 1.f;
	float green = 1.f;
	float blue = 1.f;
	float alpha = 1.f;
	char pad1[8];
	float unknown = 1.f;
	char pad2[4];
	bool renderOccluded = true;
	bool renderUnoccluded = false;
	bool fullBloom = false;
	char pad3[5];
	int glowStyle;

	GlowStruct(float r, float g, float b, float a)
		: red(r), green(g), blue(b), alpha(a)
	{

	}
};

namespace Glow {
	void run();
}
