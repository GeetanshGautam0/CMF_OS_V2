#pragma once
#include "TextPrint.cpp"
#include "typedefs.h"
#include "Control.cpp"
#include "Debuggers.cpp"

#define BUFFER_LENGTH 1000
#define HORIZONTAL 0
#define VERTICAL 1

struct Overlay {
    uint_8 minX;
    uint_8 maxX;
    uint_8 minY;
    uint_8 maxY;

    bool SystemOverlay = true;
    int OverlayDirection = HORIZONTAL;
};

const uint_8 maxOverlays = 5;
Overlay* overlays [maxOverlays];
uint_8 overlays_in_use = 0;

char* history_buffer    [BUFFER_LENGTH];     // <1000> lines (STRINGS) in buffer
char* screen_buffer     [VGA_HEIGHT];        
char line_buffer        [VGA_WIDTH * 2];     // Char, Color, Char, Color, ...
uint_8 first_line = 0;
uint_8 last_line = VGA_HEIGHT - 1; 

uint_8 CLEAR_COLOR = BACKGROUND_BLACK | FOREGROUND_WHITE;

void __setup_screen_buffer() {
    // Overlays are taken care of elsewhere (when setting last_line)
    //  [history_buffer]:
    // {<...>, <first line> ...... <last line (first + 24), <...>}
    //         ^^^^^^^^^^          ^^^^^^^^^   
    //         [first_line]        [last_line] 
    //        ===> [screen_buffer]

    uint_8 index;
    for (int row = 0; row < VGA_HEIGHT; row++) {
        index = first_line + row;
        if (index >= BUFFER_LENGTH) index = BUFFER_LENGTH - index;
        if (index > last_line) break;

        screen_buffer[row] = history_buffer[index];
    }

    return;
}

void output_screen_buffer() {
    // [history_buffer]:
    // [**1000] {<line buffer>, <line buffer>, ..}
    //            ===> Screen buffer (DONE; __setup_screen_buffer)
    // 
    // [screen_buffer]:
    // [**25] {<line buffer>, <line buffer>, ...}
    // 
    // [line_buffer]:
    // [**80] {{CHAR, COLOR}, {CHAR, COLOR}, ...}
    //          ^^^   ^^^^
    //        b800x   b800(x + 1)       ==> x % 2 == 0

    __setup_screen_buffer();
    unsigned char* mem_addr;
    for (int v = 0; v <= VGA_HEIGHT; v++) {
        // 25 line buffer (screen_buffer)

        for (int h = 0; h <= VGA_WIDTH * 2; h+=2) {
            mem_addr = (VGA_MEMORY + v*VGA_WIDTH*2 + h);
            *(mem_addr) = screen_buffer[v][h];
            *(mem_addr + 1) = screen_buffer[v][h + 1];
            if (DEB_SP_OUTPUT_SBUFFER) 
                Debugger_SP_OutputScreenBuffer(
                    mem_addr, mem_addr + 1,
                    screen_buffer[v][h], screen_buffer[v][h + 1],
                    v, h
                );
        }
    }
}

void clear_screen_history(uint_8 color = CLEAR_COLOR, bool updateDisplay = false) {
    for (int i = 0; i < VGA_WIDTH*2; i+=2) {
        line_buffer[i] = 0x20;
        line_buffer[i + 1] = color;
    }
    for (int i = 0; i < BUFFER_LENGTH; i++) history_buffer[i] = line_buffer;

    first_line = 0;
    last_line = VGA_HEIGHT - 1;

    if (updateDisplay) output_screen_buffer();
}


void add_overlay(Overlay* overlay) {
    if (overlays_in_use >= maxOverlays) return;

    overlays[overlays_in_use] = overlay;
    overlays_in_use++;
}
