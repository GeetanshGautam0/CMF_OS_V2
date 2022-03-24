#include "TextPrint.cpp"
#include "typedefs.h"
#include "IDT.cpp"
#include "Control.cpp"
#include "UI.cpp"
#include "KB.cpp"
#include "MemoryMap.cpp"
#include "Heap.cpp"
#include "Tests.cpp"

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

	const bool malloc_test_res = BOOT_malloc_test();
	PrintString("\n\rRunning Tests:\n\r");
	PrintString("\t[1] Memory allocate (malloc): ");
	PrintString(malloc_test_res ? "PASS" : "FAIL", malloc_test_res ? OkayColor : FailColor, true);

	PrintString(PBText);
	MainKeyboardHandler = KBHandler;
	AwaitKBInput(0x00, true);
	
	ClearScreen();
	UpdateStatus(DefaultStatus);
	
	SetKBEnabledStatus(true);


	return;
}

