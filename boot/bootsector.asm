[bits 16]
[org 0x7c00]
KERNEL_OFFSET equ 0x1000

	mov [BOOT_DRIVE], dl
	mov bp, 0x9000
	mov sp, bp

	mov bx, MSG_REAL_MODE
	call print_str

	call load_kernel

	call protected_switch

	jmp $


%include "boot/print_str.asm"
%include "boot/print_hex.asm"
%include "boot/protected_print_str.asm"
%include "boot/protected_switch.asm"
%include "boot/gdt.asm"
%include "boot/disk.asm"


[bits 16]
load_kernel:
	mov bx, KERNEL_OFFSET
	mov dh, 2
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret

[bits 32]
protected_begin:
	mov ebx, MSG_PROTECTED_MODE
	call protected_print_str

	call KERNEL_OFFSET

	jmp $


BOOT_DRIVE db 0

MSG_REAL_MODE:
	db "Initializing boot in 16-bit real mode",13 ,10 ,0

MSG_PROTECTED_MODE:
	db "Transitioned to 32-bit protected mode",0

; padding
times 510 - ($-$$) db 0
dw 0xaa55
