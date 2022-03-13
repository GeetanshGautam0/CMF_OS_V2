; Origin for the following code
[org 0x7e00]

jmp EnterProtectedMode	

; 16-bit includes
%include "print.asm"
%include "gdt.asm"

EnterProtectedMode:
	; Enable A20 Line (backwards compatability stuff)
	call EnableA20		
	;
	; Disable interupts
	cli
	;
	; Load GDT (Global Descriptor Table)
	lgdt [gdt_descriptor]
	; 32-bit protected mode
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp codeseg:StartProtectedMode

EnableA20:
	in al, 0x92
	or al, 2
	out 0x92, al

	ret

[bits 32]

; 32-bit includes
%include "CPUID.asm"
%include "simplepaging.asm"

StartProtectedMode:

	mov ax, dataseg
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	; Can't use 'PrintString' anymore; must move things into video mem manually
	; Starting video mem. address = 0xb8000
	; Increment addresses by 2, NOT 1

	mov [0xb8000], byte 'T'
	mov [0xb8002], byte 'E'
	mov [0xb8004], byte 'S'
	mov [0xb8006], byte 'T'


	call DetectCPUID				; Check for CPU ID
	call DetectLongMode				; Check for Long Mode
	call SetupIdentityPaging		; Enter Long Mode

	call EditGDT					; Edit GDT to a 64 bit GDT

	jmp codeseg:Start64Bit			; Jump to 64 bit code


[bits 64]
Start64Bit:
	; BLUE SCREEN
	mov edi, 0xb8000
	mov rax, 0x1f201f201f201f20		; 20 = Space, 1 = white foreground, f = blue background
	mov ecx, 500
	rep stosq

	jmp $							; Inf Loop (Stop here)

; Fill those final sectors as needed
times 2048-($-$$) db 0