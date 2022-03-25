#pragma once
#include "TextPrint.cpp"
#include "typedefs.h"
#include "Control.cpp"

uint_8 StatusColor = BACKGROUND_BLINKING_BLUE | FOREGROUND_LIGHT_GRAY;
uint_8 StatusConstantColor = BACKGROUND_BLUE | FOREGROUND_LIGHT_GRAY;
uint_8 StatusTitleColor = BACKGROUND_BLUE | FOREGROUND_LIGHT_CYAN;
uint_8 BackgroundColor = BACKGROUND_BLACK | FOREGROUND_WHITE;

uint_8 OkayColor = BACKGROUND_BLACK | FOREGROUND_GREEN;
uint_8 FailColor = BACKGROUND_BLACK | FOREGROUND_RED;

const char* DefaultStatus = "Ready";
const char* KBDisabledStatus = "KEYBOARD DISABLED";
char* CurrentStatus;

bool __init_ui = false;

void UpdateStatus(const char * Status, bool blinking=true) {
	if (SYS_ERR) return;
	if (!UI_LEGACY) return;

	const char * name_string = "Geetansh Gautam";
	if (Status == DefaultStatus) Status = !DIS_INPUT ? Status : KBDisabledStatus;
	SysCommand = true;

	uint_16 ogPos = CursorPosition;
	FillRow(24, StatusConstantColor);
	SetCursorPosition(PositionFromCoords(0, 24));
	PrintString(Status, ((blinking && !DIS_INPUT) ? StatusColor : StatusConstantColor));
	SetCursorPosition(PositionFromCoords(65, 24));
	PrintString(name_string, StatusConstantColor);
	SetCursorPosition(ogPos);

	CurrentStatus = (char*)Status;

	SysCommand = false;
}

void InitUI() {
	if (!__legacy_mode) return;
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
	__init_ui = true;

}
