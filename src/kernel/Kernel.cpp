#include "TextPrint.cpp"
#include "typedefs.h"
#include "IDT.cpp"
#include "Control.cpp"
#include "UI.cpp"
#include "KB.cpp"
#include "MemoryMap.cpp"

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

	UpdateStatus("Waiting...");
	PrintString(PBText);
	MainKeyboardHandler = KBHandler;
	AwaitKBInput(0x00, true);
	
	PRINT_KB_INPUT = true;
	DEBUG_PRINT_SCAN_CODE = false;
	ClearScreen();
	UpdateStatus(DefaultStatus);
	
	return;
}

