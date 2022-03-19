#include "TextPrint.cpp"
#include "typedefs.cpp"
#include "IDT.cpp"

extern const char PBText[];

uint_8 StatusColor = BACKGROUND_BLINKING_BLUE | FOREGROUND_LIGHT_GRAY;
uint_8 StatusConstantColor = BACKGROUND_BLUE | FOREGROUND_LIGHT_GRAY;
uint_8 StatusTitleColor = BACKGROUND_BLUE | FOREGROUND_LIGHT_CYAN;
uint_8 BackgroundColor = BACKGROUND_BLACK | FOREGROUND_WHITE;

uint_8 lastKBScanCode = NULL;
bool KBToggle = false;
bool PRINT_KB_INPUT = true;
bool DEBUG_PRINT_SCAN_CODE = false;

const char * DefaultStatus = "Ready";

bool LeftShiftPressed = false;
bool RightShiftPressed = false;

void UpdateStatus(const char * Status, bool blinking=true) {
	const char * name_string = "by Geetansh Gautam";

	SysCommand = true;

	uint_16 ogPos = CursorPosition;
	FillRow(24, StatusConstantColor);
	SetCursorPosition(PositionFromCoords(0, 24));
	PrintString(Status, (blinking ? StatusColor : StatusConstantColor));
	SetCursorPosition(PositionFromCoords(62, 24));
	PrintString(name_string, StatusConstantColor);
	SetCursorPosition(ogPos);

	SysCommand = false;
}

void InitUI() {
	const char * title_string = "CMF OS m.2";
	const char * status = "Initializing ...";

	SysCommand = true;

	ClearScreen(BackgroundColor, true, true);

	// Create Status Bar
	FillRow(24, StatusConstantColor);
	FillRow(23, StatusConstantColor);
	SetCursorPosition(PositionFromCoords((uint_8)(VGA_WIDTH/2 - sizeof(title_string)/2), 23));
	PrintString(title_string, StatusTitleColor);
	SetCursorPosition(0);

	UpdateStatus(status);

	SysCommand = false;

}

uint_8 AwaitKBInput(uint_8 ScanCode, bool AnyKey = true) {
    bool org = KBToggle;
    while (lastKBScanCode == NULL || (KBToggle == org && (AnyKey || lastKBScanCode == ScanCode))) continue;
    return lastKBScanCode;
}

void stdKBHandler(uint_8 scanCode, uint_8 chr) {
	KBToggle = !KBToggle;

	if (PRINT_KB_INPUT)
	{
		switch(scanCode) {
			case 0x8E: {
				Backspace();
				break;
			} case 0x2a : {
				LeftShiftPressed = true;
				UpdateStatus("SHIFT", false);
				break;
			} case 0xAA : {
				LeftShiftPressed = false;
				UpdateStatus(DefaultStatus);
				break;
			} case 0x36 : {
				RightShiftPressed = true;
				UpdateStatus("SHIFT");
				break;
			} case 0xB6 : {
				RightShiftPressed = false;
				UpdateStatus(DefaultStatus);
				break;
			} case 0x9c: {
				PrintString("\n\r");
				break;
			}
		}
	}
	if (chr != 0)
	{
		switch (LeftShiftPressed || RightShiftPressed) {
			case true:
				PrintChar(chr - 32);
				break;
			case false:
				PrintChar(chr);
				break;
		}
	}

    if (DEBUG_PRINT_SCAN_CODE) PrintString(HexToString(scanCode), BACKGROUND_BLINKING_RED | FOREGROUND_WHITE);
    return;
}

void KBHandler0xE0(uint_8 scanCode) {
	switch (scanCode)
	{
		case 0x50: { // Down arrow
			SetCursorPosition(CursorPosition + VGA_WIDTH);
			break;
		}
		case 0x48: { // Down arrow
			SetCursorPosition(CursorPosition - VGA_WIDTH);
			break;
		}
		
		default:
			break;
	}
}

void KBHandler(uint_8 scanCode, uint_8 chr) {
	
	switch (lastKBScanCode) {
		case 0xe0 : {				// Arrows
			KBHandler0xE0(scanCode);
			break;
		}

		default:
			stdKBHandler(scanCode, chr);
	}
	
	lastKBScanCode = scanCode;
	return;
}

extern "C" void _start() {
	SetCursorPosition(PositionFromCoords(0, 0));
	PrintString("CMF OS m.2\n\rTesting ...\n\r");
	PrintString(HexToString(0x1234abcd), BACKGROUND_BLINKING_BLACK | FOREGROUND_CYAN);
	PrintString("\n\r");
	PrintString(HexToString("HI")); 
	PrintString("\n\r");

	for (int i = 0; i < 100000; i++) {
		SetCursorPosition(PositionFromCoords(0, 6));
		PrintString(IntToStr(i));
	}
	
	InitUI();
	
	ClearScreen(BackgroundColor);
	UpdateStatus("Initializing IDT...");
	InitializeIDT();

	ClearScreen(BackgroundColor);
	UpdateStatus("Waiting...");

	PrintString(PBText);
	MainKeyboardHandler = KBHandler;
	AwaitKBInput(0x00, true);

	ClearScreen();
	UpdateStatus(DefaultStatus);

	return;
}

