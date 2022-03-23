#pragma once
#include "TextPrint.cpp"
#include "UI.cpp"
#include "typedefs.cpp"
#include "TextModeColorCodes.cpp"

void ErrStatusBar(const char* status) {
    SysCommand = true;

    const char * cr_text = "Geetansh Gautam"; 
    const uint_8 MainColor = BACKGROUND_RED | FOREGROUND_LIGHT_GRAY;
    const uint_8 MainBlinkingColor = BACKGROUND_BLINKING_RED | FOREGROUND_LIGHT_GRAY;
    const uint_8 TitleColor = BACKGROUND_RED | FOREGROUND_WHITE;

    FillRow(24, MainColor);
    FillRow(23, MainColor);

    PrintCenteredText("CMF OS", 23, TitleColor);
    SetCursorPosition(PositionFromCoords(0, 24));
    PrintString(status, MainBlinkingColor);
    SetCursorPosition(PositionFromCoords(65, 24));
    PrintString(cr_text, MainColor);

    SysCommand = false;
}

void HALT() { while (true); }

void ShowFatalErrorScreen(const char* title, int errorCode, const char* text = "") {    

    ClearScreen(BACKGROUND_BLACK | FOREGROUND_LIGHT_RED);
    FillRow(0, BACKGROUND_RED | FOREGROUND_WHITE);
    FillRow(1, BACKGROUND_RED | FOREGROUND_WHITE);
    FillRow(2, BACKGROUND_RED | FOREGROUND_WHITE);

    ErrStatusBar("ERROR");
    PrintCenteredText(title, 1, BACKGROUND_RED | FOREGROUND_WHITE);
    SetCursorPosition(PositionFromCoords(0, 3));
    PrintString("Error Code: ");
    PrintString("0x", BACKGROUND_BLACK | FOREGROUND_YELLOW);
    PrintString(HexToString(errorCode), BACKGROUND_BLACK | FOREGROUND_YELLOW);

    SetCursorPosition(PositionFromCoords(0, 4));
    PrintString("This error is "); 
    PrintString("FATAL", BACKGROUND_BLACK | FOREGROUND_YELLOW);
    PrintString("; the system needs to be restarted.");

    SetCursorPosition(PositionFromCoords(0, 6));
    PrintString(text);
   
    SYS_ERR = true;

    HALT();
}
