#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <comdef.h>

#include "Offsets.h"
#include "Globals.h"

#include "Features/Glow.h"
#include "Features/AutoBH.h"
#include "Features/Triggerbot.h"
#include "Features/RadarHack.h"
#include "Vector.h"
#include "Features/Aimbot.h"

#define VERSION "1.1"

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

int main() {
	std::cout << "Opsok External v" << VERSION << std::endl;
	hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
	if (!hwnd) {
		std::cout << "CS:GO window not found.";
		//std::getchar();
		return 0;
	}

	GetWindowThreadProcessId(hwnd, &procId);
	moduleBase = GetModuleBaseAddress(procId, L"client.dll");
	engineModule = GetModuleBaseAddress(procId, L"engine.dll");
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	std::cout <<"HWND: " << hwnd << std::endl << "ModuleBase: " << moduleBase << std::endl;

	while (true)
	{
		Glow::run();
		AutoBH::run();
		Triggerbot::run();
		RadarHack::run();
		Aimbot::run();
	}
}
