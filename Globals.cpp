#include "Offsets.h"
#include "Globals.h"
#include "Memory.h"
#include <cstdint>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

uintptr_t clientBase = 0;
uintptr_t engineBase = 0;
DWORD procId = 0;
HWND hwnd = 0;
HANDLE hProcess = 0;
Memory mem;
Config config;
