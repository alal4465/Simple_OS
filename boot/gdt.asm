gdt_base:

null_descriptor:
    dd 0x0
    dd 0x0

code_descriptor: 
    dw 0xffff    ; segment limit (bits 0-15)
    dw 0x0       ; segment base (bits 0-15)
    db 0x0       ; segment base (bits 16-23)
    db 10011010b ; flags (bits 0-15)
    db 11001111b ; flags (bits 16-19)
    db 0x0       ; segment base (bits 24-31)

data_descriptor:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b ; flags here differ. executable bit is off
    db 11001111b
    db 0x0

gdt_limit:

gdt_descriptor:
    dw gdt_limit - gdt_base - 1 ; gdt size
    dd gdt_base                 ; gdt addr

; define constants to be used in segment registers
CODE_SEG equ code_descriptor - gdt_base
DATA_SEG equ data_descriptor - gdt_base
