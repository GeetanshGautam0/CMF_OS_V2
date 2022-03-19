#include "TextPrint.cpp"

extern "C" void _start() {
	SetCursorPosition(PositionFromCoords(0, 0));
	PrintString("CMF OS m.2\n\rGeetansh Gautam\n\r");
	PrintString(HexToString(0x1234abcd));
	PrintString("\n\r");
	PrintString(HexToString("HI")); 
	return;
}
