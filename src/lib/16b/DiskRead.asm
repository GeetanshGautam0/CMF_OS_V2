LoadDisk:
    push dx

    mov ah, 0x02                                ; BIOS read sector function
    mov al, dh                                  ; Number of sectors to be loaded
    mov ch, 0x00                                ; Cylinder 0
    mov dh, 0x00                                ; Head 0
    mov cl, 0x02                                ; Start reading from the 2nd sector
                                                ; (the sector after the boot sector)
    int 0x13                                    ; Interrupt to read
    jc _disk_read_error                         ; Error

    pop dx
    cmp dh, al
    jne _disk_read_error_2

    ret

_disk_read_error:
    mov bx, DISK_READ_ERROR_MSG                 ; Load error message
    call PrintString                            ; Print the error string
    
    jmp $                                       ; Stay here

_disk_read_error_2:
    mov bx, DISK_READ_ERROR_MSG                 ; Load error message
    call PrintString                            ; Print the error string
    
    jmp $                                       ; Stay here

; Variables
DISK_READ_ERROR_MSG db 'Failed to read disk (1) [FATAL]', 0
DISK_READ_ERROR_MSG_2 db 'Failed to read disk (2) [FATAL]', 0
BOOT_DISK db 0
