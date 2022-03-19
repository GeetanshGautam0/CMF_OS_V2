#pragma once
#include "IO.cpp"
#include "typedefs.cpp"
#include "TextModeColorCodes.cpp"

#define VGA_MEMORY (unsigned char*)0xb8000
#define VGA_WIDTH 80
#define NULL 0

#define MAX_X VGA_WIDTH
#define MAX_Y 22

uint_16 CursorPosition;
uint_8 ScreenColor = BACKGROUND_BLACK | FOREGROUND_WHITE;

bool SysCommand = false;

void SetCursorPosition(uint_16 position) {
    if (position > 2000) position = 2000;
    else if (position < 0) position = 0;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint_8)(position & 0xff));
    outb(0x3d4, 0x0e);
    outb(0x3d5, (uint_8)((position >> 8) & 0xFF));

    CursorPosition = position;
}

uint_16 PositionFromCoords(uint_8 x, uint_8 y) {
    if (!SysCommand) {
        if (x > MAX_X) x = MAX_X;
        else if (x < 0) x = 0;

        if (y > MAX_Y) y = MAX_Y;
        else if (y < 0) y = 0;
    }
    return y * VGA_WIDTH + x;
}

void PrintString(const char* str, uint_8 color = ScreenColor, unsigned char* MEM_ADDR = NULL) {
    uint_8* charPtr = (uint_8*)str;
    uint_16 index = CursorPosition;
    unsigned char* addrAdd = 0x00000;

    while (*charPtr != 0) {
        switch (*charPtr) {
            case 10: {                              // Newline (\n) == 10
                index += VGA_WIDTH;
                break;
            }
            case 13: {                              // CR (\r) == 13
                index -= index % VGA_WIDTH;
                break;
            }
            default: {
                *(VGA_MEMORY + index * 2) = *charPtr;
                *(VGA_MEMORY + index * 2 + 1) = color;
                index++;
            }
        }

        charPtr++;
    }

    SetCursorPosition(index);
}

void ClearScreen(uint_64 ClearColor = BACKGROUND_BLACK | FOREGROUND_WHITE, bool ResetCursorPos = true, bool ClearStatus = false)
{
    uint_64 value = 0;
    value += ClearColor << 8;
    value += ClearColor << 24;
    value += ClearColor << 40;
    value += ClearColor << 56;

    for (uint_64* i = (uint_64*)VGA_MEMORY; i < (uint_64*)(VGA_MEMORY + ((SysCommand && ClearStatus) ? 4000 : 3680)); i++) {
        *i = value;
    }

    ScreenColor = ClearColor;

    if (ResetCursorPos) SetCursorPosition(0);

}

unsigned char* MemAddressFromCoords(uint_8 x, uint_8 y) {
    return (unsigned char*)(0xb8000 + y*0x00002*VGA_WIDTH + x*0x00002);
}

char hexToStringOutput[128]; 						// Buffer (?)
template<typename T>
const char* HexToString(T value) {
	T* valPtr = &value;
	uint_8* ptr;
	uint_8 temp;
	uint_8 size = (sizeof(T)) * 2 - 1;
	uint_8 i;
	for (i = 0; i < size; i++) {
		ptr = ((uint_8*)valPtr + i);
		temp = ((*ptr & 0xF0) >> 4);
		hexToStringOutput[size - (i*2+1)] = temp + (temp > 9 ? 55: 48);
		temp = ((*ptr & 0x0F));
		hexToStringOutput[size - (i*2+0)] = temp + (temp > 9 ? 55: 48);
	}
	hexToStringOutput[size + 1] = 0;				// Null termination
	return hexToStringOutput;
}

void FillRow(uint_8 RowIndex, uint_8 Color) {
    if (!SysCommand) {
        if (RowIndex > MAX_Y) return;
        else if (RowIndex < 0) return;
    }

    uint_8* rowStart = MemAddressFromCoords(0, RowIndex);
    uint_8* rowEnd   = MemAddressFromCoords(VGA_WIDTH, RowIndex);

    for (uint_8* i = rowStart; i < rowEnd; i+=2) {
        *i = 0x00;
        *(i + 1) = Color;
    }

}
