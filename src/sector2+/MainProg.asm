[org 0x7e00]

mov bx, EntryString
call PrintString

jmp EnterProtectedMode

EntryString db 'Loaded MainEntry', NEWLINE, CR, 0
EntringPMString db 'Entering 32 bit protected mode', NEWLINE, CR, 0

%include "src/lib/16b/Print.asm"
%include "src/lib/16b/GDT.asm"

EnterProtectedMode:
    ; Disable Interrupts
    ; Enable A20 Line           (Backwards compatability)
    ; Load the GDT

    mov bx, EntringPMString
    call PrintString

    call ENB_A20                ; Enables A20
    cli                         ; Disables Iterrupts
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1 
    mov cr0, eax

    jmp codeseg:StartProtectedMode

ENB_A20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

[bits 32]

%include "src/lib/32b PM/Vars.asm"
%include "src/lib/32b PM/CPUID.asm"
%include "src/lib/32b PM/SimplePaging.asm"

StartProtectedMode:

    mov ax, dataseg
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov [0xb8000], byte '1'
    call DetectCPUID
    
    mov [0xb8000], byte '2'
    call DetectLongMode

    mov [0xb8000], byte '3'
    call SetupIdentityPaging

    mov [0xb8000], byte '4'
    call EditGDT

    jmp codeseg:Start64Bit

[bits 64]
%include "src/lib/64b/Vars.asm"

Start64Bit:
    mov edi, VIDEO_MEMORY
    mov rax, 0x30202f060207020
    mov ecx, 500
    rep stosq

    jmp $

times 2048-($-$$) db 0
