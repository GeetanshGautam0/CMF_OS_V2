[org 0x7c00]                ; Sets origin to loaded boot sector (see os-dev.pdy pg. 18)
mov [BOOT_DISK], dl         ; Which drive we booted from

; Stack
mov bp, 0x7c00              ; Stack base pointer
mov sp, bp                  ; indirectly set that to sp

; Startup
mov bx, startup_text        ; Load startup text
call PrintString            ; Display startup text

; Loader
call ReadDisk               ; Actually read the data
jmp PROGRAM_SPACE

; Variables and declearations
startup_text db 'Booting CMF OS m.2 by Geetansh Gautam', NEWLINE, CR, 0

; External Files
%include "src/lib/16b/Print.asm"
%include "src/lib/16b/DiskRead.asm"

times 510-($-$$) db 0       ; Fill all bytes prior to the 510th with a zero
                            ; (excluding those produced by the code priro)
                            ; The two last bytes are the "magic number"

dw 0xaa55                   ; dw = declare word (two bytes)
                            ; 0xaa55 lets the BIOS know that this is a boot
                            ; sector.

; [PROGRAM_SPACE] (0x7e00)
