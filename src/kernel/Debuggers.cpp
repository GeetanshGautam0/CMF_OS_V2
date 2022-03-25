#pragma once
#include "typedefs.h"
#include "TextPrint.cpp"

void Debugger_SP_OutputScreenBuffer (uint_8* ma1, uint_8* ma2, char c, char C, int v, int h) {
	SetCursorPosition(0);
	PrintString(HexToString(ma1)); PrintString(" "); PrintString(HexToString(c), ScreenColor, true);
	PrintString(HexToString(ma2)); PrintString(" "); PrintString(HexToString(C), ScreenColor, true);
	PrintString("Row "); PrintString(IntToStr(v), ScreenColor, true);
	PrintString("Column "); PrintString(IntToStr(h), ScreenColor, true);
}
