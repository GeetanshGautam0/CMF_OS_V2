#pragma once
#include "typedefs.h"

struct MemorySegmentHeader {
    uint_64 MemoryLength;
    MemorySegmentHeader* NextSegment;
    MemorySegmentHeader* PreviousSegment;
    MemorySegmentHeader* NextFreeSegment;
    MemorySegmentHeader* PreviousFreeSegment;

    bool Free;
};

MemorySegmentHeader* FirstFreeMemSegment;

void InitHeap(uint_64 heapAddress, uint_64 heapLength) {
    FirstFreeMemSegment = (MemorySegmentHeader*)heapAddress;
    FirstFreeMemSegment->MemoryLength = heapLength - sizeof(MemorySegmentHeader);
    FirstFreeMemSegment->NextSegment = 0;
    FirstFreeMemSegment->PreviousSegment = 0;
    FirstFreeMemSegment->NextFreeSegment = 0;
    FirstFreeMemSegment->PreviousFreeSegment = 0;
    FirstFreeMemSegment->Free = true;
}

void* malloc(uint_64 size) {
    uint_64 remainder = size % 8;
    size -= remainder;
    if (remainder != 0) size += 8;

    MemorySegmentHeader* currentMemSegment = FirstFreeMemSegment;

    while (true) {
        if (currentMemSegment->MemoryLength >= size) {
            if (currentMemSegment->MemoryLength > size + sizeof(MemorySegmentHeader)) {
                MemorySegmentHeader* newSegmentHeader = (MemorySegmentHeader*)((uint_64)currentMemSegment + sizeof(MemorySegmentHeader) + size);

                newSegmentHeader->Free = true;
                newSegmentHeader->MemoryLength = ((uint_64)currentMemSegment->MemoryLength) - (sizeof(MemorySegmentHeader) + size);
                newSegmentHeader->NextFreeSegment = currentMemSegment->NextFreeSegment;
                newSegmentHeader->NextSegment = currentMemSegment->NextSegment;
                newSegmentHeader->PreviousSegment = currentMemSegment;
                newSegmentHeader->PreviousFreeSegment = currentMemSegment->PreviousFreeSegment;

                currentMemSegment->NextFreeSegment = newSegmentHeader;
                currentMemSegment->NextSegment = newSegmentHeader;
                currentMemSegment->MemoryLength = size;
            }

            if (currentMemSegment == FirstFreeMemSegment) {
                FirstFreeMemSegment = currentMemSegment->NextFreeSegment;
            }
            currentMemSegment->Free = false;

            if (currentMemSegment->PreviousFreeSegment != 0) currentMemSegment->PreviousFreeSegment->NextFreeSegment = currentMemSegment->NextFreeSegment;
            if (currentMemSegment->NextFreeSegment != 0) currentMemSegment->NextFreeSegment->PreviousFreeSegment = currentMemSegment->PreviousFreeSegment;
            if (currentMemSegment->PreviousSegment != 0) currentMemSegment->PreviousSegment->NextFreeSegment = currentMemSegment->NextFreeSegment;
            if (currentMemSegment->NextSegment != 0) currentMemSegment->NextSegment->PreviousFreeSegment = currentMemSegment->PreviousFreeSegment;

            return currentMemSegment + 1;
        }

        if (currentMemSegment->NextFreeSegment == 0)
        {
            return 0;   // NO MORE MEMORY.
        }
        currentMemSegment = currentMemSegment->NextFreeSegment;
    }

    return 0;   // Should never get here.
}
