#include <vector>

#include "statistics.h"
#include "mem.h"

GameStatistics gameStats;
MenuStatistics menuStats;

void maxAllStats()
{
    menuStats.strength = 9999;
    menuStats.agility = 9999;
    menuStats.endurance = 9999;
    menuStats.willpower = 9999;
}

bool initGameStatistics(bool bForced)
{
    uintptr_t baseAddress = (uintptr_t)GetModuleInfo(L"Door in the Woods.exe").lpBaseOfDll;
    for (;;)
    {
        gameStats.posx = (float*)FindDMAAddy(baseAddress + 0x01A71CC8, { 0x1C0, 0x50, 0x20, 0xE8, 0x50, 0x20, 0x50 });
        if (gameStats.posx == 0 && !bForced)
        {
            return false;
        }
        if ((void*)gameStats.posx != nullptr && (*gameStats.posx < -1 || *gameStats.posx > 1) && *gameStats.posx < 1000 && *gameStats.posx > -1000) { break; }
    }
    gameStats.experience = (double*)FindDMAAddy(baseAddress + 0x01A71CC8, { 0x1C0, 0x50, 0x20, 0xE8, 0x50, 0x20, 0x560 });
    gameStats.stamina =    (double*)FindDMAAddy(baseAddress + 0x01A71CC8, { 0x1C0, 0x50, 0x20, 0xE8, 0x50, 0x20, 0x398 });
    gameStats.sanity =     (double*)FindDMAAddy(baseAddress + 0x01A71CC8, { 0x1C0, 0x50, 0x20, 0xE8, 0x50, 0x20, 0x3E0 });
    gameStats.health =     (double*)FindDMAAddy(baseAddress + 0x01A71CC8, { 0x1C0, 0x50, 0x20, 0xE8, 0x50, 0x20, 0x2A8 });
    gameStats.posx =        (float*)FindDMAAddy(baseAddress + 0x01A71CC8, { 0x1C0, 0x50, 0x20, 0xE8, 0x50, 0x20, 0x50  });
    gameStats.posy =        (float*)FindDMAAddy(baseAddress + 0x01A71CC8, { 0x1C0, 0x50, 0x20, 0xE8, 0x50, 0x20, 0x54  });
    gameStats.strength =      (int*)FindDMAAddy(baseAddress + 0x01A71CC8, { 0x1C0, 0x50, 0x20, 0xE8, 0x50, 0x20, 0x500 });
    gameStats.agility =       (int*)FindDMAAddy(baseAddress + 0x01A71CC8, { 0x1C0, 0x50, 0x20, 0xE8, 0x50, 0x20, 0x518 });
    gameStats.endurance =     (int*)FindDMAAddy(baseAddress + 0x01A71CC8, { 0x1C0, 0x50, 0x20, 0xE8, 0x50, 0x20, 0x530 });
    gameStats.willpower =     (int*)FindDMAAddy(baseAddress + 0x01A71CC8, { 0x1C0, 0x50, 0x20, 0xE8, 0x50, 0x20, 0x548 });
    return true;
}