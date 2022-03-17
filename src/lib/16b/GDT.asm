gdt_nulldesc:		; Null Descriptor
	dd 0
	dd 0

gdt_codedesc:		; Code Section
	dw 0xffff		; Limit
	dw 0x0000		; Base (low)

	db 0x00			; Base (Medium)
	db 10011010b	; Flags
	db 11001111b	; More Flags + Upper Limit
	db 0x00			; Base High

gdt_datadesc:		; Data Section
	; Most of the code is the same, but we need to
	; change the executable bit from 1 to 0 in order
	; to declare the data section (found in "Flags")

	dw 0xffff		; Limit
	dw 0x0000		; Base (low)

	db 0x00			; Base (Medium)
	db 10010010b	; Flags (Exec. bit is 5th bit)
	db 11001111b	; More Flags + Upper Limit
	db 0x00			; Base High

gdt_end:

gdt_descriptor:		; GDT Descriptor
	gdt_size:
		dw gdt_end - gdt_nulldesc - 1	; Have to subtract one
		dd gdt_nulldesc


; Constants
codeseg equ gdt_codedesc - gdt_nulldesc
dataseg equ gdt_datadesc - gdt_nulldesc

[bits 32]
EditGDT:
	mov [gdt_codedesc + 6], byte 10101111b
	mov [gdt_datadesc + 6], byte 10101111b
	
    ret

[bits 16]