;
;   A simple boot sector program that loops forever (prevents machine from crasing)
;   Credit: PDF atteched to git repo.
;

; loop:
;   jmp loop

[org 0x7c00]                ; Sets origin to loaded boot sector (see os-dev.pdy pg. 18)

; mov ah, 0x0e              ; Magic number (?)
; mov al, 'H'               ; Store the byte 'H' in a low
; int 0x10                  ; Call interrupt 0x10 (print?)

mov bx, example_string
call PrintString

jmp $                       ; Jump to current mem addresss (inf. loop)

example_string:
    db 'Hello, World!', 0   ; 0 = Null termination

PrintString:
    mov ah, 0x0e            ; Magic

    .print_char:
        cmp [bx], byte 0    ; Check for null termination
        je .exit_print      ; If null, go to `.exit_print`
                            ; Otherwise...
        mov al, [bx]        ; (1) Move char to al
        int 0x10            ; (2) Interrupt to print ?
        inc bx              ; (3) Increment to next character
        jmp .print_char     ; (4) Repeat until done

    .exit_print:
        ret                 ; Return


times 510-($-$$) db 0       ; Fill all bytes prior to the 510th with a zero
                            ; (excluding those produced by the code priro)
                            ; The two last bytes are the "magic number"

dw 0xaa55                   ; dw = declare word (two bytes)
                            ; 0xaa55 lets the BIOS know that this is a boot
                            ; sector.