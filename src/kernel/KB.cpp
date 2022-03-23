#pragma once
#include "TextPrint.cpp"
#include "typedefs.cpp"
#include "Control.cpp"
#include "UI.cpp"
#include "ErrorScreen.cpp"
// #include "string.cpp"
#include "ErrorCodes.cpp"

int StringCPP_StrLen (const char * str) {
    int i = 0;
    for(; str[i] != 0; i++);
    return i;
}

char * StringCPP_Concat (const char * str1, const char * str2, const char delim = ' ') {

	const int len1 = StringCPP_StrLen(str1);
	const int len2 = StringCPP_StrLen(str2);

	// char arr [len1 + len2 + 2];
	// PrintString(IntToStr(sizeof(arr)), ScreenColor, true);
	PrintString(IntToStr(len1), ScreenColor, true);
	PrintString(IntToStr(len2), ScreenColor, true);

	// PrintString("\n\r");

	// for (int i = 0; i < len1; i ++) arr[i] = str1[i];
	// for (int i = 0; i < len2; i ++) arr[len1 + i] = str2[i];
	// arr[len1] = delim;
	// arr[len1 + len2 + 1] = '\0';

	return "";

}

uint_8 _shift_alpha(uint_8 chr) { return chr - 32; }
uint_8 _shift_non_alpha (uint_8 chr) {
	uint_8 newchr = chr;
	if (chr == 0x31) 						newchr = 0x21;
	else if (chr == 0x32)					newchr = 0x40;
	else if (0x33 <= chr && chr <= 0x35)	newchr = chr - 0x10;
	else if (chr == 0x36)					newchr = 0x5e;
	else if (chr == 0x37)					newchr = 0x26;
	else if (chr == 0x38)					newchr = 0x2a;
	else if (chr == 0x39)					newchr = 0x28;
	else if (chr == 0x30)					newchr = 0x29;
	else if (chr == 0x5b || chr == 0x5d)	newchr = chr + 0x20;
	else if (chr == 0x60)					newchr = 0x7e;
	else if (chr == 0x2d)					newchr = 0x5f;
	else if (chr == 0x3d)					newchr = 0x2b;
	else if (chr == 0x5c)					newchr = 0x7c;
	else if (chr == 0x3b)					newchr = 0x3a;
	else if (chr == 0x27)					newchr = 0x22;
	else if (chr == 0x2c || chr == 0x2e || chr == 0x2f) newchr = chr + 0x10;

	return newchr;

}

uint_8 ShiftMode(uint_8 scanCode, uint_8 chr) {
	if (0x61 <= chr && chr <= 0x7a) 		return _shift_alpha(chr);
	else if (!CapsLock)						return _shift_non_alpha(chr);
	else									return chr;
}

uint_8 AwaitKBInput(uint_8 ScanCode, bool AnyKey = true) {
    bool org = KBToggle;
    while (lastKBScanCode == 0 || (KBToggle == org && (AnyKey || lastKBScanCode == ScanCode))) continue;
    return lastKBScanCode;
}

void PrintSKStatus() {
	bool old [3] = {KB_SpecialKeys[0], KB_SpecialKeys[1], KB_SpecialKeys[2]};
	KB_SpecialKeys[KB_SK_SHIFT] = (LeftShiftPressed || RightShiftPressed);
	KB_SpecialKeys[KB_SK_ALT]	= ALTPressed;
	KB_SpecialKeys[KB_SK_CTRL]	= CTRLPressed;

	if (old == KB_SpecialKeys) {
		return;
	}

	ClearScreen();

	char * txt = "";

	if (CTRLPressed) txt = "CTRL";
	// PrintString("1 ");
	// PrintString(txt);
	// PrintString("\n\r");

	if (LeftShiftPressed || RightShiftPressed) {
		PrintString(StringCPP_Concat("Hello,", "World!"), true);
		txt = ((sizeof(txt) > 0) ? StringCPP_Concat(txt, "SHIFT", '+'): (char*)"SHIFT");
	}

	// PrintString("2 ");
	// PrintString(txt);
	// PrintString("\n\r");

	if (ALTPressed) {
		txt = ((sizeof(txt) > 0) ? StringCPP_Concat(txt, "ALT", '+'): (char*)"ALT");
	}

	// PrintString("3 ");
	// PrintString(txt);
	// PrintString("\n\r");

	if (LeftShiftPressed || RightShiftPressed || CTRLPressed || ALTPressed) 
		// UpdateStatus(txt, false);
		PrintString(txt);
}

void stdKBHandler(uint_8 scanCode, uint_8 chr) {
	KBToggle = !KBToggle;

	if (PRINT_KB_INPUT)
	{
		switch(scanCode) {
			case 0x0e: {
				Backspace();
				break;
			} case 0x2a : {
				LeftShiftPressed = true;
				break;
			} case 0xAA : {
				LeftShiftPressed = false;
				break;
			} case 0x36 : {
				RightShiftPressed = true;
				break;
			} case 0xB6 : {
				RightShiftPressed = false;
				break;
			} case 0x1c: {
				PrintString("\n\r");
				break;
			} case 0x3A : {
				CapsLock = !CapsLock;
				if (CapsLock) UpdateStatus("CAPS LOCK", false);
				else UpdateStatus(DefaultStatus);
				break;
			} case 0xff : {
				const char * errText = "The error was caused during the handling of a keyboard event; additional information is unavaliable due to the nature of the error.";
				ShowFatalErrorScreen("FATAL ERROR", KEYBOARD_TEMPLATE | scanCode, errText);
				break;
			} case 0x00 : {
				const char * errText = "The error was caused during the handling of a keyboard event; additional information is unavaliable due to the nature of the error.";
				ShowFatalErrorScreen("FATAL ERROR", KEYBOARD_TEMPLATE | scanCode, errText);
				break;
			} case 0x1d : {
				CTRLPressed = true;
				break;
			} case 0x9d : {
				CTRLPressed = false;
				break;
			} case 0x38 : {
				ALTPressed = true;
				break;
			} case 0xb8 : {
				ALTPressed = false;
				break;
			}
		}
	}
	
	PrintSKStatus();

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

void KBHandler_ArrowKeys(uint_8 scanCode) {
	switch (scanCode)
	{
		case 0x50: { // Down arrow
			SetCursorPosition(CursorPosition + VGA_WIDTH);
			break;
		}
		case 0x48: { // Up arrow
			SetCursorPosition(CursorPosition - VGA_WIDTH);
			break;
		}
		case 0x4b: {
			SetCursorPosition(CursorPosition - 1);
			break;
		} 
		case 0x4d: {
			SetCursorPosition(CursorPosition + 1);
			break;
		}
		
		default:
			break;
	}
}

void KBHandler(uint_8 scanCode, uint_8 chr) {
	
	if (DIS_INPUT) return;

	switch (lastKBScanCode) {
		case 0xe0 : {				// Arrows
			KBHandler_ArrowKeys(scanCode);
			break;
		}

		default:
			stdKBHandler(scanCode, chr);
	}
	
	lastKBScanCode = scanCode;
	return;
}
