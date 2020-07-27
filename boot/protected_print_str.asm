[bits 32] 

VIDEO_MEMORY equ 0xb8000
WHITE_OB_BLACK equ 0x0f

; print a null-byte terminated string in 32-bit protected mode
protected_print_str:
    pusha
    mov edx, VIDEO_MEMORY

.loop:
    mov al, [ebx] ; grab a byte from the string
    mov ah, WHITE_OB_BLACK

    cmp al, 0 ; check if end of string
    je .return

    mov [edx], ax ; print char
    add ebx, 1
    add edx, 2

    jmp .loop

.return:
    popa
    ret
