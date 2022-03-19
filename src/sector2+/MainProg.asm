
jmp EnterProtectedMode

%include "src/lib/16b/GDT.asm"
%include "src/lib/16b/Print.asm"


EnterProtectedMode:
	mov bx, EnteringText
	call PrintString

	call EnableA20
	cli
	lgdt [gdt_descriptor]
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp codeseg:StartProtectedMode

EnableA20:
	in al, 0x92
	or al, 2
	out 0x92, al
	ret

EnteringText db 'Entering 32-bit protected mode', NEWLINE, CR, NULL

[bits 32]

%include "src/lib/32b PM/CPUID.asm"
%include "src/lib/32b PM/SimplePaging.asm"

StartProtectedMode:

	mov ax, dataseg
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov [0xb8000], byte '1'
	call DetectCPUID
	mov [0xb8000], byte '2'
	call DetectLongMode
	mov [0xb8000], byte '3'
	call SetUpIdentityPaging
	mov [0xb8000], byte '4'
	call EditGDT
	mov [0xb8000], byte '5'
	jmp codeseg:Start64Bit

[bits 64]

[extern _start]

Start64Bit:
	mov edi, 0xb8000
	mov rax, 0x0E200E200E200E20
	mov ecx, 500
	rep stosq

	call _start
	
	jmp $

times 2048-($-$$) db 0
