PROGRAM_SPACE equ 0x7e00

ReadDisk:
    mov ah, 0x02                        ; BIOS function to read the disk
    mov bx, PROGRAM_SPACE               ; Tell BIOS where to put the new data
    mov al, 4                           ; Load 4 sectors
    mov dl, [BOOT_DISK]                 ; Which disk to read from
    mov ch, 0x00                        ; Cylinder 0
    mov dh, 0x00                        ; Head 0
    mov cl, 0x02                        ; Read from sector 2

    int 0x13                            ; Reads data
    jc DR_fail

    call DR_success

    ret


DR_fail:
    mov bx, DISK_READ_FAILURE_STR
    call PrintString

    jmp $

DR_success:
    mov bx, DISK_READ_SUCCESS_STR
    call PrintString

    ret

; Variables
DISK_READ_FAILURE_STR db 'Failed to read disk (0)', NEWLINE, CR, 0
DISK_READ_SUCCESS_STR db 'Successfully read disk (0)', NEWLINE, CR, 0
BOOT_DISK db 0
