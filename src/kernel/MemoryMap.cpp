#pragma once
#include "typedefs.h"
#include "TextPrint.cpp"

struct MemoryMapEntry {
    uint_64 BaseAddress;
    uint_64 RegionLength;
    uint_32 RegionType;
    uint_32 ExtendedAttr;
};

extern uint_8 MemoryRegionCount;
uint_8 UsableMemRegionCount;

MemoryMapEntry* UsableMemoryRegions[10];

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint_16 position) {
    SetCursorPosition(position);
    uint_8 c = ScreenColor;
    PrintString("Memory Base: "); PrintString(IntToStr(memoryMap->BaseAddress), c, true);
    PrintString("   Region Length: "); PrintString(IntToStr(memoryMap->RegionLength), c, true);
    PrintString("   Memory Type: "); PrintString(IntToStr(memoryMap->RegionType), c, true);
    PrintString("   Memory Attributes: "); PrintString(HexToString(memoryMap->ExtendedAttr), c, true);
}

bool MemoryRegionsGot = false;
MemoryMapEntry** GetUsableMemoryRegions() {
    if (MemoryRegionsGot) return UsableMemoryRegions;
    
    uint_8 UsableRegionIndex;
    for (uint_8 i = 0; i < MemoryRegionCount; i++) {
        MemoryMapEntry* memMap = (MemoryMapEntry*)0x5000;
        memMap += i;
        if (memMap->RegionType == 1) { // Usable
            UsableMemoryRegions[UsableRegionIndex] = memMap;
            UsableRegionIndex++;
        }
    }

    MemoryRegionsGot = true;
    UsableMemRegionCount = UsableRegionIndex;

    return UsableMemoryRegions;
}
