TestString:
	db 'Hello, World!', 0	; Test string

PrintString:
	mov ah, 0x0e			; Magic Number; no idea what it does

	.print_char:			; Starting point to print_char loop
		cmp [bx], byte 0	; Check for null-termination (current char)
		je .exit_print		; If null, jump to exit point

		; Loop code
		mov al, [bx]		; Move char to al
		int 0x10			; Interrupt to print from al
		inc bx				; Increment char
		jmp .print_char		; Go back to the top

	.exit_print:			; Exit point
		ret					; Return