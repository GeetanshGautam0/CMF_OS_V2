
DetectCPUID:

    pushfd
    pop eax

    mov ecx, eax            ; Stores original flag
    xor eax, 1 << 21        ; Flip flag

    push eax                ; Push flag back 
    popfd                   ; from eax

    pushfd                  ; Get the flag
    pop eax                 ; back and store to eax
    
    push ecx                ; Restore original flag
    popfd                   ; status from ecx

    xor eax, ecx            ; XOR the two flags
    je NoCPUID              ; If different, XOR=1; 
                            ; CPUID not available.
    ret


DetectLongMode:
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz NoLongMode

    ret

NoLongMode:
    hlt                     ; Cannot continue

NoCPUID:
    hlt                     ; Cannot continue
