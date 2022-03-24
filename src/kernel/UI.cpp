#pragma once
#include "TextPrint.cpp"
#include "typedefs.h"
#include "Control.cpp"

uint_8 StatusColor = BACKGROUND_BLINKING_BLUE | FOREGROUND_LIGHT_GRAY;
uint_8 StatusConstantColor = BACKGROUND_BLUE | FOREGROUND_LIGHT_GRAY;
uint_8 StatusTitleColor = BACKGROUND_BLUE | FOREGROUND_LIGHT_CYAN;
uint_8 BackgroundColor = BACKGROUND_BLACK | FOREGROUND_WHITE;

const char* DefaultStatus = "Ready";
const char* KBDisabledStatus = "KEYBOARD DISABLED";
char* CurrentStatus;

void UpdateStatus(const char * Status, bool blinking=true) {
	if (SYS_ERR) return;

	const char * name_string = "Geetansh Gautam";
	if (Status == DefaultStatus) Status = PRINT_KB_INPUT ? Status : KBDisabledStatus;
	SysCommand = true;

	uint_16 ogPos = CursorPosition;
	FillRow(24, StatusConstantColor);
	SetCursorPosition(PositionFromCoords(0, 24));
	PrintString(Status, ((blinking && PRINT_KB_INPUT) ? StatusColor : StatusConstantColor));
	SetCursorPosition(PositionFromCoords(65, 24));
	PrintString(name_string, StatusConstantColor);
	SetCursorPosition(ogPos);

	CurrentStatus = (char*)Status;

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
