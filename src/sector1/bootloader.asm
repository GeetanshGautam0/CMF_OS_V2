
[org 0x7c00]

mov [BOOT_DISK], dl

mov bp, 0x7c00
mov sp, bp

mov bx, StartStr
call PrintString
mov bx, PreLoadStr
call PrintString

call ReadDisk

mov bx, PostLoadStr
call PrintString

jmp PROGRAM_SPACE

%include "src/lib/16b/Print.asm"
%include "src/lib/16b/DiskRead.asm"

StartStr db 'CMF OS m.2 by Geetansh Gautam - Booting', NEWLINE, CR, NULL
PreLoadStr db 'Loading MainProg from disk', NEWLINE, CR, NULL
PostLoadStr db 'Loaded MainProg -- attempting to execute', NEWLINE, CR, NULL

times 510-($-$$) db 0

dw 0xaa55