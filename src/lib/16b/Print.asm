NEWLINE equ 0xA
CR equ 0xD
NULL equ 0

PrintString:
	push ax
	push bx

	mov ah, 0x0e
	.Loop:
	cmp [bx], byte NULL
	je .Exit
		mov al, [bx]
		int 0x10
		inc bx
		jmp .Loop
	.Exit:

	pop ax
	pop bx
	ret