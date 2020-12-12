#include <iostream>
#include <string>
#include <thread>

#include <Windows.h>
#include <TlHelp32.h>
#include <comdef.h>

#include "Offsets.h"
#include "Globals.h"

#include "Features/Glow.h"
#include "Features/AutoBH.h"
#include "Features/Triggerbot.h"
#include "Features/RadarHack.h"
#include "Features/Aimbot.h"

#define VERSION "1.2"
#define print(text) std::cout << text << std::endl

void userInput() {
	while (true) {
		std::cout << "> ";
		std::string cmd;
		std::getline(std::cin, cmd);

		if (cmd == "help") {
			print("No commands available (yet).");
		}
		else {
			print("Unknown command. Type \"help\" for a list of commands.");
		}

	}
}

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
	system("color B");
	std::cout << "Opsok External v" << VERSION << std::endl;
	hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
	if (!hwnd) {
		std::cout << "CS:GO window not found." << std::endl << "Waiting for CS:GO..." << std::endl;
		do {
			Sleep(1000);
			hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
		} while (!hwnd);
	}

	GetWindowThreadProcessId(hwnd, &procId);
	clientBase = GetModuleBaseAddress(procId, L"client.dll");
	engineBase = GetModuleBaseAddress(procId, L"engine.dll");
	if (!clientBase || !engineBase) {
		do {
			Sleep(1000);
			clientBase = GetModuleBaseAddress(procId, L"client.dll");
			engineBase = GetModuleBaseAddress(procId, L"engine.dll");
		} while (!clientBase || !engineBase);
	}
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	std::cout << "CS:GO window HWND: " << hwnd << std::endl << "client.dll base: " << clientBase << std::endl << "engine.dll base: " << engineBase << std::endl << std::endl;

	std::thread userInputThread(userInput);

	while (true) {
		Glow::run();
		AutoBH::run();
		Triggerbot::run();
		RadarHack::run();
		Aimbot::run();
	}
}
