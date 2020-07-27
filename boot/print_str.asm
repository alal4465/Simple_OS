; print a NULL-Terminated string pointed to by bx
print_str:
	pusha
	mov ah, 0x0e

.loop:
	mov al, [bx] ; grab a char from the string
	cmp al, 0 
	je .return

	int 0x10     ; print the char

	add bx, 1
	jmp .loop

.return:
	popa
	ret
