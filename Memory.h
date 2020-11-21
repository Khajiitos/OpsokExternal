#pragma once
#include <cstdint>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "Globals.h"

class Memory {
public:
	template<typename T> 
	T read(SIZE_T address);

	template<typename T>
	void write(SIZE_T address, T buffer);
};

template<typename T>
T Memory::read(SIZE_T address) {
	T buffer;
	ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), NULL);
	return buffer;
}

template<typename T>
void Memory::write(SIZE_T address, T buffer) {
	WriteProcessMemory(hProcess, (LPVOID)address, &buffer, sizeof(buffer), NULL);
}
