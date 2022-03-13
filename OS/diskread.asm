
PROGRAM_SPACE equ 0x7e00

ReadDisk:
	mov ah, 0x02			; BIOS function to read a disk

	mov bx, PROGRAM_SPACE
	mov al, 4				; Num. sectors

	mov dl, [BOOT_DISK]		; Which Drive
	mov ch, 0x00			; Cylinder 0
	mov dh, 0x00			; Head 0
	mov cl, 0x02			; Sector 2

	int 0x13				; Read
	jc DiskReadFailed		; If disk read fails

	ret

DiskReadErrorString:
	db 'Disk Read Faled', 0

DiskReadFailed:
	mov bx, DiskReadErrorString
	call PrintString

	jmp $					; Jump to current address (inf. loop)

BOOT_DISK:
	db 0
