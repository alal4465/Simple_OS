; print the value of bx as hex
print_hex:
    	pusha
	mov ah, 0x0e

	mov al, '0'   ; print hex "0x" prefix
	int 0x10

	mov al, 'x'
	int 0x10

	mov cx , 4    ; cx is a counter
	rol bx , 4    ; rotate bx to print in correct order
.loop:

	mov dx, bx    ; grab a byte from bx
	and dx, 0xf

	mov al, dl
	
	cmp al,0xA    ; add 0x37 if value is numeric else add 0x30
	jl .is_digit

	add al, 0x37	
	jmp .print_char

.is_digit:
	add al, 0x30

.print_char:
	int 0x10      ; print char

	dec cx
	cmp cx, 0
	je .return	
	
	rol bx, 4
	jmp .loop

.return:
	
	mov al, 13    ; print a new line and a carrige return
	int 0x10

	mov al, 10
	int 0x10

    	popa
	ret
