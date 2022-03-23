#include "TextPrint.cpp"
#include "typedefs.cpp"
#include "IDT.cpp"
#include "Control.cpp"
#include "UI.cpp"
#include "KB.cpp"

extern const char PBText[];

extern "C" void _start() {
	InitUI();
	
	ClearScreen(BackgroundColor);
	UpdateStatus("Initializing IDT...");
	InitializeIDT();

	ClearScreen(BackgroundColor);
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

