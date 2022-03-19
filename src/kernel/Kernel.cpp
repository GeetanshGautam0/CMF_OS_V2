#include "TextPrint.cpp"
#include "typedefs.cpp"

extern const char PBText[];

uint_8 StatusColor = BACKGROUND_BLINKING_BLUE | FOREGROUND_LIGHT_GRAY;
uint_8 StatusConstantColor = BACKGROUND_BLUE | FOREGROUND_LIGHT_GRAY;
uint_8 StatusTitleColor = BACKGROUND_BLUE | FOREGROUND_LIGHT_CYAN;
uint_8 BackgroundColor = BACKGROUND_BLACK | FOREGROUND_WHITE;

void UpdateStatus(const char * Status) {
	const char * name_string = "by Geetansh Gautam";

	SysCommand = true;

	uint_16 ogPos = CursorPosition;
	FillRow(24, StatusConstantColor);
	SetCursorPosition(PositionFromCoords(0, 24));
	PrintString(Status, StatusColor);
	SetCursorPosition(PositionFromCoords(65, 24));
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

extern "C" void _start() {
	SetCursorPosition(PositionFromCoords(0, 0));
	PrintString("CMF OS m.2\n\rTesting ...\n\r");
	PrintString(HexToString(0x1234abcd), BACKGROUND_BLINKING_BLACK | FOREGROUND_CYAN);
	PrintString("\n\r");
	PrintString(HexToString("HI")); 
	PrintString("\n\r");

	for (int i = 0; i < 10000; i++) continue;

	InitUI();
	UpdateStatus("Ready");

	PrintString(PBText);

	return;
}

