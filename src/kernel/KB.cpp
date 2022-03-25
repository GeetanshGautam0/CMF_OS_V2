#pragma once
#include "TextPrint.cpp"
#include "typedefs.h"
#include "Control.cpp"
#include "UI.cpp"
#include "ErrorScreen.cpp"
#include "string.cpp"
#include "ErrorCodes.cpp"

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
    bool org = (bool)(int)KBToggle;
	UpdateStatus("Waiting...");
    while (lastKBScanCode == 0 || (KBToggle == org && (AnyKey || lastKBScanCode == ScanCode))) continue;
    return lastKBScanCode;
}

void SetKBPrintStatus(bool KB_PRINT_STATUS) {
	PRINT_KB_INPUT = KB_PRINT_STATUS;
}

void SetKBEnabledStatus(bool KB_STATUS) {
	DIS_INPUT = !KB_STATUS;
	if (CurrentStatus == DefaultStatus || CurrentStatus == KBDisabledStatus)
		UpdateStatus(DefaultStatus);
}

void PrintSKStatus() {
	bool old [3] = {KB_SpecialKeys[0], KB_SpecialKeys[1], KB_SpecialKeys[2]};
	KB_SpecialKeys[KB_SK_SHIFT] = (LeftShiftPressed || RightShiftPressed);
	KB_SpecialKeys[KB_SK_ALT]	= ALTPressed;
	KB_SpecialKeys[KB_SK_CTRL]	= CTRLPressed;

	if (old == KB_SpecialKeys) {
		return;
	}

	char * txt = "";

	if (CTRLPressed) txt = "CTRL";
	if (LeftShiftPressed || RightShiftPressed) {
		if (StrLen(txt) > 0) {
			char arr [VGA_WIDTH - 15];
			txt = StrConcat(txt, "SHIFT", arr, '+');
		} else txt = "SHIFT";
	}
	if (ALTPressed) {
		if (StrLen(txt) > 0) {
			char arr [VGA_WIDTH - 15];
			txt = StrConcat(txt, "ALT", arr, '+');
		} else txt = "ALT";
	}

	if (LeftShiftPressed || RightShiftPressed || CTRLPressed || ALTPressed) 
		UpdateStatus(txt, false);
	else UpdateStatus(DefaultStatus);
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


    if (DEBUG_PRINT_SCAN_CODE) PrintString(HexToString(scanCode), BACKGROUND_BLINKING_CYAN | FOREGROUND_BLUE);
    return;
}

void stdKBHandler_ArrowKeys(uint_8 scanCode) {
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

void NLKBHandler_ArrowKeys(uint_8 scanCode) {} 

void NLKBHandler(uint_8 scanCode, uint_8 chr) {}

void KBHandler(uint_8 scanCode, uint_8 chr) {
	
	if (DIS_INPUT) return;

	if (UI_LEGACY)
	{
		switch (lastKBScanCode) {
			case 0xe0 : {				// Arrows
				stdKBHandler_ArrowKeys(scanCode);
				break;
			}

			default:
				stdKBHandler(scanCode, chr);
		}
	}
	else 
	{
		switch (lastKBScanCode) {
			case 0x0e0 : {
				NLKBHandler_ArrowKeys(scanCode);
				break;
			}

			default:
				NLKBHandler(scanCode, chr);
		}
	}
	lastKBScanCode = scanCode;
	return;
}
