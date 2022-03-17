;
;   A simple boot sector program that loops forever (prevents machine from crasing)
;   Credit: PDF atteched to git repo.
;

[org 0x7c00]                ; Sets origin to loaded boot sector (see os-dev.pdy pg. 18)
mov [BOOT_DISK], dl         ; Which drive we booted from

; Stack
mov bp, 0x8000              ; Stack base pointer
mov sp, bp                  ; indirectly set that to sp

; Startup
mov bx, startup_text        ; Load startup text
call PrintString            ; Display startup text

; Loader
mov bx, 0x9000              ; Move 5 sectors (dh) to 0x0000(ES):0x9000(BX)
mov dh, 5                   ; from the boot disk ([BOOT_DISK])

mov dl, [BOOT_DISK]         ; Which disk to read from

call LoadDisk               ; Actually read the data

mov dx, [0x9000]            ; Print our first loaded word (dada)
call PrintHex               ; [DEFINED AT THE END OF THIS FILE]

mov dx, [0x9000 + 512]      ; Print the first word (2 bytes) from the
call PrintHex               ; 2nd loaded sector (0xface) [DEFINED AT END
                            ; OF THIS FILE]

;
; ------------------------------------------------------------------------------------------
;

; END POINT
jmp $                       ; Jump to current mem addresss (inf. loop)

; Variables and declearations
startup_text db 'Booting CMF OS m.2 by Geetansh Gautam', 0

; External Files
%include "src/lib/16b/Print.asm"
%include "src/lib/16b/DiskRead.asm"

times 510-($-$$) db 0       ; Fill all bytes prior to the 510th with a zero
                            ; (excluding those produced by the code priro)
                            ; The two last bytes are the "magic number"

dw 0xaa55                   ; dw = declare word (two bytes)
                            ; 0xaa55 lets the BIOS know that this is a boot
                            ; sector.

; Sample words
times 256 dw 0xdada
times 256 dw 0xface
