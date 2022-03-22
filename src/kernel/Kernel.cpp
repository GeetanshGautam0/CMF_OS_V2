#include "TextPrint.cpp"
#include "typedefs.cpp"
#include "IDT.cpp"
// #include "string.cpp"

extern const char PBText[];

uint_8 StatusColor = BACKGROUND_BLINKING_BLUE | FOREGROUND_LIGHT_GRAY;
uint_8 StatusConstantColor = BACKGROUND_BLUE | FOREGROUND_LIGHT_GRAY;
uint_8 StatusTitleColor = BACKGROUND_BLUE | FOREGROUND_LIGHT_CYAN;
uint_8 BackgroundColor = BACKGROUND_BLACK | FOREGROUND_WHITE;

uint_8 lastKBScanCode = (const uint_8)NULL;
bool KBToggle = false;
bool PRINT_KB_INPUT = true;
bool DEBUG_PRINT_SCAN_CODE = false;

const string DefaultStatus = "Ready";

bool LeftShiftPressed = false;
bool RightShiftPressed = false;
bool CapsLock = false;

uint_8 ShiftMode(uint_8 scanCode, uint_8 chr) {
	// Default = original character
	uint_8 newchr = chr;
	
	// Alphabet
	if (0x61 <= chr <= 0x7a) 					newchr = chr - 32; 
	
	if (!CapsLock)	// DO NOT DO THE FOLLLOWING IF CAPS IS ON.
	{	
		// Numbers ==> Symbols
		if (chr == 0x31) 						newchr = 0x21;
		if (chr == 0x32)					newchr = 0x40;
		if (0x33 <= chr <= 0x35)			newchr = chr - 0x10;
		if (chr == 0x36)					newchr = 0x5e;
		if (chr == 0x37)					newchr = 0x26;
		if (chr == 0x38)					newchr = 0x2a;
		if (chr == 0x39)					newchr = 0x28;
		if (chr == 0x30)					newchr = 0x29;

		// [] ==> {}
		if (chr == 0x5b || chr == 0x5d)	newchr = chr + 0x20;

		// ` ==> ~
		if (chr == 0x60)					newchr = 0x7e;

		// -, = ==> _, +
		if (chr == 0x2d)					newchr = 0x5f;
		if (chr == 0x3d)					newchr = 0x2b;

		// \ ==> (pipe)
		if (chr == 0x5c)					newchr = 0x7c;

		// ;, ' ==> :, "
		if (chr == 0x3b)					newchr = 0x3a;
		if (chr == 0x27)					newchr = 0x22;

		// ,, ., / ==> <, >, ?
		if (chr == 0x2c || chr == 0x2e || chr == 0x2f) newchr = chr + 0x10;

	}
	return newchr;
}

void UpdateStatus(const char * Status, bool blinking=true) {
	const char * name_string = "by Geetansh Gautam";
	Status = PRINT_KB_INPUT ? Status : "KEYBOARD DISABLED";
	SysCommand = true;

	uint_16 ogPos = CursorPosition;
	FillRow(24, StatusConstantColor);
	SetCursorPosition(PositionFromCoords(0, 24));
	PrintString(Status, ((blinking && PRINT_KB_INPUT) ? StatusColor : StatusConstantColor));
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
			} case 0x3A : {
				CapsLock = !CapsLock;
				if (CapsLock) UpdateStatus("CAPS LOCK", false);
				else UpdateStatus(DefaultStatus);
				break;
			}
		}
	}

	if (chr != 0 && PRINT_KB_INPUT)
	{
		switch ((LeftShiftPressed || RightShiftPressed) != CapsLock) {
			case true:
				PrintChar(ShiftMode(scanCode, chr));
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
	PRINT_KB_INPUT = true;

	// const string OG = "Hello, World!";
	// const string delim = " ";

	// string* p = StrTok(OG, delim);
	// PrintString(IntToStr(sizeof(p)));

	ClearScreen();
	UpdateStatus(DefaultStatus);

	return;
}

