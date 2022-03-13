[org 0x7c00]

mov [BOOT_DISK], dl
; Move the number of the boot disk into BOOT_DISK

mov bp, 0x7c00
mov sp, bp
; Stack pointer stuff

call ReadDisk
; Read from disk

jmp PROGRAM_SPACE
; Program space is defined as 0x7e00 in diskread.asm
; This is the address where MainEntry.asm begins
; Essentially, we jump to the MainEntry file.

jmp $
; Jump to current mem address (inf. loop)

; Includes
%include "print.asm"
%include "diskread.asm"

times 510-($-$$) db 0
; Fill the appropriate amount of space (510)

dw 0xaa55
; Needed to run the code (BIOS)
