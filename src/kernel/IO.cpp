#pragma once

void outb(unsigned short port, unsigned char value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
    return;
}

unsigned char inb(unsigned short port) {
    unsigned char retrunVal;
    asm volatile ("inb %1, %0"
    : "=a"(retrunVal)
    : "Nd"(port));

    return retrunVal;
}
