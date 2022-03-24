#include "TextPrint.cpp"
#include "typedefs.h"
#include "IDT.cpp"
#include "Control.cpp"
#include "UI.cpp"
#include "KB.cpp"
#include "MemoryMap.cpp"
#include "Heap.cpp"

extern const char PBText[];

extern "C" void _start() {
	InitUI();
	
	ClearScreen(BackgroundColor);
	UpdateStatus("Initializing IDT...");
	InitializeIDT();

	ClearScreen(BackgroundColor);
	SetCursorPosition(PositionFromCoords(0, 0));

	MemoryMapEntry** UsableMemMaps = GetUsableMemoryRegions();
	for (uint_8 i = 0; i < UsableMemRegionCount; i++) {
		MemoryMapEntry* memMap = UsableMemMaps[i];
		PrintMemoryMap(memMap, CursorPosition);
	}
	PRINT_KB_INPUT = false;
	DEBUG_PRINT_SCAN_CODE = false;

	PrintString("Initializing Heap...\n\r"); 
	InitHeap(0x100000, 0x100000);
	PrintString(PBText);
	
	MainKeyboardHandler = KBHandler;
	AwaitKBInput(0x00, true);
	
	ClearScreen();
	UpdateStatus(DefaultStatus);
	
	SetKBEnabledStatus(true);

	void* TestMemoryAddress = malloc(0x10);
	PrintString("1"); 
	void* TestMemoryAddress2 = malloc(0x10);
	PrintString("2"); 
	void* TestMemoryAddress3 = malloc(0x10);
	PrintString("3"); 
	
	SetCursorPosition(0);
	PrintString(HexToString((uint_64)TestMemoryAddress), ScreenColor, true);
	PrintString(HexToString((uint_64)TestMemoryAddress2), ScreenColor, true);
	PrintString(HexToString((uint_64)TestMemoryAddress3), ScreenColor, true);

	return;
}

