#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <vector>

MODULEINFO GetModuleInfo(LPCWSTR szModule);
bool IsValidReadPtr(void* Ptr);
uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
void UpdateValues();
void Teleport(int index);