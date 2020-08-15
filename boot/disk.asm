; load dh sectors from driver dl to memory pointed to by bx
disk_load:
    pusha
    push dx

    mov ah, 2  ; interrupt
    mov al, dh ; num of sectors
    mov cl, 2  ; sector 2 and beyond
    mov ch, 0  ; cylinder 0
    mov dh, 0  ; head 0

    int 0x13
    jc disk_err   ; error stored in carry bit

    pop dx

    cmp al, dh    ; cmp number of sectors read
    jne sectors_err

    popa
    ret


disk_err:
    mov bx, DISK_ERROR
    call print_str

    mov bh, ah ; ah = error code
    mov bl, dl ; dl = faulty disk drive
    call print_hex
    jmp disk_loop

sectors_err:
    mov bx, SECTORS_ERROR
    call print_str

disk_loop:
    jmp $

DISK_ERROR: db "err while reading disk", 10, 13 , 0
SECTORS_ERROR: db "wrong amount of sectors read", 10, 13, 0
