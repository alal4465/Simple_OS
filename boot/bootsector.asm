[bits 16]
[org 0x7c00]

	mov bp, 0x8000
	mov sp, bp

	mov bx, MSG_REAL_MODE
	call print_str

	call protected_switch

	jmp $


%include "print_str.asm"
%include "print_hex.asm"
%include "protected_print_str.asm"
%include "protected_switch.asm"
%include "gdt.asm"
;%include "enter_kernel.asm"
%include "disk.asm"


[bits 32]
protected_begin:
	mov ebx, MSG_PROTECTED_MODE
	call protected_print_str
	jmp $


MSG_REAL_MODE:
	db "Initializing boot in 16-bit real mode",13 ,10 ,0

MSG_PROTECTED_MODE:
	db "Transitioned to 32-bit protected mode",0

; padding
times 510 - ($-$$) db 0
dw 0xaa55
