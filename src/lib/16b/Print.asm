NEWLINE equ 0xA
CR equ 0xD

PrintString:
    ; string to print in bx
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


PrintHex:
    ; hex to print in dx
    pusha
    mov si, HEX_OUT + 2
    
    mov cx, 0               ; Counter

ph_next_char:
    inc cx                  ; Increment counter for each nibble

    ; Isolate the current nibble
    mov bx, dx
    and bx, 0xf000          ; & 15
    shr bx, 4               ; >> 4

    ; Add 0x30 to get the ASCII digit value
    add bh, 0x30

    ; If hex > 9, bh > 0x39 (check); add 7 to get
    ; ASCII letters
    cmp bh, 0x39
    jg ph_add_7

ph_add_character_hex:
    ; put the current nibble into the string template (HEX_OUT)
    mov [si], bh

    ; incerment the template string's char position
    inc si

    shl dx, 4

    cmp cx, 4 
    jnz ph_next_char
    jmp ph_done

ph_add_7:
    add bh, 0x7
    jmp ph_add_character_hex

ph_done:
    mov bx, HEX_OUT
    call PrintString

    popa
    ret

HEX_OUT: db '0x0000', 0
