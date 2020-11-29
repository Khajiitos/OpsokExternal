#pragma once
#include <cstdint>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

extern uintptr_t moduleBase;
extern uintptr_t engineModule;
extern DWORD procId;
extern HWND hwnd;
extern HANDLE hProcess;

#include "Memory.h"
extern Memory mem;

enum Flags {
	ON_GROUND = (1 << 0),
};

enum Team {
	SPECTATORS = 1,
	TERRORISTS,
	COUNTERTERRORISTS
};

struct Config {
	bool glow = true;
	bool autoBh = true;
	bool triggerbot = true;
	short triggerbotKey = VK_XBUTTON2;
	bool radarHack = true;
	bool aimbot = true;
	short aimbotToggleKey = VK_XBUTTON1;
};

extern Config config;
