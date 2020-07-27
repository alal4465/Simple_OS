[bits 16]
protected_switch:
	cli  		  ; disable interrupts

	lgdt [gdt_descriptor] ; load the GDT

	mov eax, cr0
	or eax, 0x1 	  ; set PE bit in cr0
	mov cr0, eax

	jmp CODE_SEG:protected_init ; far jump by using the code segment from the gdt

[bits 32]
protected_init:
	mov ax, DATA_SEG   ; update the segment registers
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000  ; update the stack
	mov esp, ebp

	call protected_begin
