#pragma once
#include "Heap.cpp"
#include "typedefs.h"
#include "TextPrint.cpp"

bool BOOT_malloc_test() {
    uint_64 TestAddr1 = (uint_64)malloc(0x10);
    uint_64 TestAddr2 = (uint_64)malloc(0x10);
    uint_64 TestAddr3 = (uint_64)malloc(0x10);
    return (TestAddr1 == (uint_64)0x100030) && (TestAddr2 == (uint_64)0x100070) && (TestAddr3 == (uint_64)0x1000B0);
}
