#include <Windows.h>
#include <Psapi.h>
#include <vector>

#include "mem.h"
#include "statistics.h"

MODULEINFO GetModuleInfo(LPCWSTR szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

#define MEM_WRITE (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)
#define MEM_READ (PAGE_EXECUTE_READ | PAGE_READONLY | MEM_WRITE)

bool IsValidReadPtr(void* Ptr)
{
	if (!Ptr)
		return false; //Ptr is a nullptr

	MEMORY_BASIC_INFORMATION MBI{ 0 };
	if (!VirtualQuery(Ptr, &MBI, sizeof(MEMORY_BASIC_INFORMATION)))
		return false; //VirtualQuery fail = definitly not a valid pointer

	if ((MBI.State == MEM_COMMIT) && (MBI.Protect & MEM_READ) && !(MBI.Protect & PAGE_GUARD))
		return true; //memory is commited, page has (at least) read access and isn't a guard page

	return false;
}

uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		if ((void*)addr == nullptr)
		{
			return 0;
		}
		if (!IsValidReadPtr((void*)addr))
		{
			return 0;
		}
		addr = *(uintptr_t*)addr;
		if (addr == 0)
		{
			return 0;
		}
		addr += offsets[i];
	}
	return addr;
}

void UpdateValues()
{
	if (!initGameStatistics(false)) { return; };
	*gameStats.experience = (double)menuStats.experience;
	*gameStats.strength = (int)menuStats.strength;
	*gameStats.agility = (int)menuStats.agility;
	*gameStats.endurance = (int)menuStats.endurance;
	*gameStats.willpower = (int)menuStats.willpower;
}

void Teleport(int index)
{
	if (!initGameStatistics(false)) { return; };
	*gameStats.posx = gameStats.posxList.at(index);
	*gameStats.posy = gameStats.posyList.at(index);
}